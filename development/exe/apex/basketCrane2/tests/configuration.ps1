param(
    [Parameter(Mandatory=$true)][string]$Executable,
    [string]$Ini = (Join-Path $PSScriptRoot '..\basketCrane2.ini')
)
$ErrorActionPreference = 'Stop'
$Executable = (Resolve-Path -LiteralPath $Executable).Path
$baseline = [regex]::Replace([IO.File]::ReadAllText((Resolve-Path -LiteralPath $Ini).Path), '(?m)^Mode=[^\r\n]*', 'Mode=Live')
$testDir = Join-Path ([IO.Path]::GetTempPath()) ('basket-config-' + [Guid]::NewGuid())
New-Item -ItemType Directory -Path $testDir | Out-Null
function Check($name, $content, $expectedExit, $expectedMessage, $defaultPath = $false, $safety = $false) {
    $iniPath = Join-Path $testDir 'test.ini'
    if ($null -ne $content) { [IO.File]::WriteAllText($iniPath, $content, [Text.UTF8Encoding]::new($false)) }
    elseif (Test-Path -LiteralPath $iniPath) { Remove-Item -LiteralPath $iniPath }
    $start = [Diagnostics.ProcessStartInfo]::new()
    $start.FileName = $Executable
    $start.Arguments = if ($safety) { '--check-safety' } else { '--check-config' }
    if (!$defaultPath) { $start.Arguments += ' --config "' + $iniPath + '"' }
    $start.WorkingDirectory = $testDir
    $start.UseShellExecute = $false
    $start.CreateNoWindow = $true
    $start.RedirectStandardOutput = $true
    $start.RedirectStandardError = $true
    $process = [Diagnostics.Process]::Start($start)
    if (!$process.WaitForExit(30000)) { $process.Kill(); throw "$name timed out" }
    $output = $process.StandardOutput.ReadToEnd() + $process.StandardError.ReadToEnd()
    if ($process.ExitCode -ne $expectedExit -or !$output.Contains($expectedMessage)) {
        throw "$name failed: exit $($process.ExitCode), output: $output"
    }
    $process.Dispose()
    Write-Output "PASS: $name"
}
try {
    Check 'SQL driver safety and instance isolation' $baseline 0 'PASS: SELECT results and bindings' $false $true
    Check 'Live permission policy' $baseline 0 'Environment=Live; Database=read/write; PLC=live'
    Check 'Test permission policy' ($baseline.Replace('Mode=Live', 'Mode=Test')) 0 'Environment=Test; Database=read-only; PLC=offline'
    Check 'LiveObserver permission policy' ($baseline.Replace('Mode=Live', 'Mode=LiveObserver')) 0 'Environment=LiveObserver; Database=read-only; PLC=read-only telemetry'
    Check 'missing environment cannot enable live control' ([regex]::Replace($baseline, '(?m)^Mode=Live\r?\n', '')) 1 'Missing configuration key: Environment/Mode'
    Check 'emulation flag cannot override policy' ($baseline + "`n[Plc]`nEmulate=false`n") 1 'Unknown configuration key'
    Check 'production INI via explicit path' $baseline 0 'Configuration is valid.'
    Check 'legacy INI without email settings' ([regex]::Replace($baseline, '(?ms)^\[EmailAlerts\].*\z', '')) 0 'Configuration is valid.'
    Check 'invalid email configuration disables alerts without blocking HMI' ($baseline.Replace('SmtpPort=25', 'SmtpPort=invalid')) 0 'Email alerts disabled: invalid [EmailAlerts] settings:'
    Check 'default INI independent of working directory' $baseline 0 'Configuration is valid.' $true
    Check 'missing file' $null 1 'Cannot read configuration file'
    Check 'missing server' ([regex]::Replace($baseline, '(?m)^Server=[^\r\n]+\r?\n', '')) 1 'Missing configuration key'
    Check 'legacy DSN rejected' ($baseline + "`n[BasketDatabase]`nDsn=obsolete`n") 1 'Unknown configuration key'
    Check 'invalid SQL endpoint' ([regex]::Replace($baseline,'(?m)^Server=[^\r\n]*','Server="bad;host"')) 1 'Invalid SQL Server endpoint'
    Check 'invalid SQL encryption' ($baseline.Replace('Encrypt=Mandatory', 'Encrypt=invalid')) 1 'Encrypt must be Mandatory, Strict, or Optional'
    Check 'invalid certificate setting' ([regex]::Replace($baseline, '(?m)^TrustServerCertificate=[^\r\n]*', 'TrustServerCertificate=invalid')) 1 'TrustServerCertificate must be true or false'
    Check 'unquoted SQL endpoint with port' ([regex]::Replace($baseline,'(?m)^Server=[^\r\n]*','Server=localhost,1433')) 0 'Configuration is valid.'
    Check 'quoted SQL endpoint with port' ([regex]::Replace($baseline,'(?m)^Server=[^\r\n]*','Server="localhost,1433"')) 0 'Configuration is valid.'
    Check 'empty password' ([regex]::Replace($baseline, '(?m)^Password=[^\r\n]+', 'Password=')) 1 'must not be empty'
    Check 'invalid IP' ($baseline.Replace('Crane2Ip=20.20.20.60', 'Crane2Ip=999.999.1.1')) 1 'Invalid PLC IP address'
    Check 'invalid environment' ($baseline.Replace('Mode=Live', 'Mode=Invalid')) 1 'must be Test, Live, or LiveObserver'
    Check 'zero polling interval' ($baseline.Replace('PollIntervalMs=100', 'PollIntervalMs=0')) 1 'positive integer'
    Check 'negative timer' ($baseline.Replace('MainMs=1000', 'MainMs=-1')) 1 'positive integer'
    Check 'integer overflow' ($baseline.Replace('RetainedRows=10000', 'RetainedRows=2147483648')) 1 'positive integer'
    Check 'invalid catalog identifier' ($baseline.Replace('Catalog=Epics', 'Catalog=Epics];drop table x;--')) 1 'Invalid database catalog identifier'
    Check 'unknown key' ($baseline + "`nUnexpected=1`n") 1 'Unknown configuration key'
    Check 'alternate deployment settings' ($baseline.Replace('Mode=Live', 'Mode=Test').Replace('MainMs=1000', 'MainMs=2000').Replace('Server=192.168.105.94\\SQLExpress', 'Server=local,1433')) 0 'Configuration is valid.'
} finally {
    # Remove only files created by these tests, using the verified directory.
    $resolvedTestDir = [IO.Path]::GetFullPath($testDir)
    if (![IO.Path]::GetFileName($resolvedTestDir).StartsWith('basket-config-') -or
        [IO.Path]::GetDirectoryName($resolvedTestDir) -ne [IO.Path]::GetTempPath().TrimEnd('\')) {
        throw 'Unexpected cleanup path'
    }
    Remove-Item -LiteralPath (Join-Path $resolvedTestDir 'test.ini') -Force -ErrorAction SilentlyContinue
    Remove-Item -LiteralPath $resolvedTestDir
}
