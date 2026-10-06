[CmdletBinding()]
param([Parameter(Mandatory=$true)][string]$Path, [hashtable]$Servers = @{})
$ErrorActionPreference = 'Stop'
$Path = (Resolve-Path -LiteralPath $Path).Path
$content = [IO.File]::ReadAllText($Path)
$updated = $content
foreach ($section in @('BasketDatabase','EpicsDatabase','Epics2Database')) {
    $pattern = '(?ms)(^\['+[regex]::Escape($section)+'\]\r?\n)(.*?)(?=^\[|\z)'
    $match = [regex]::Match($updated,$pattern)
    if (!$match.Success) { throw "Missing database section: $section" }
    $body = $match.Groups[2].Value
    $dsn = [regex]::Match($body,'(?m)^Dsn=([^\r\n]*)')
    if ($dsn.Success) {
        $server = $Servers[$section]
        if (!$server) {
            # One-time migration only. The application never uses this registry.
            foreach ($hive in @([Microsoft.Win32.RegistryHive]::CurrentUser,[Microsoft.Win32.RegistryHive]::LocalMachine)) {
                $base = [Microsoft.Win32.RegistryKey]::OpenBaseKey($hive,[Microsoft.Win32.RegistryView]::Registry32)
                try {
                    $entry = $base.OpenSubKey('SOFTWARE\ODBC\ODBC.INI\'+$dsn.Groups[1].Value.Trim())
                    if ($entry) { try { $server = $entry.GetValue('Server') } finally { $entry.Dispose() } }
                } finally { $base.Dispose() }
                if ($server) { break }
            }
        }
        if (!$server) { throw "Cannot resolve $section server. Supply -Servers @{ $section='host,port' }. No file has been changed." }
        $escapedServer = $server.Replace('\','\\')
        $body = [regex]::Replace($body,'(?m)^Dsn=[^\r\n]*',[System.Text.RegularExpressions.MatchEvaluator]{ param($unused) 'Server='+$escapedServer })
    }
    if (![regex]::IsMatch($body,'(?m)^Encrypt=')) { $body = $body.TrimEnd()+"`r`nEncrypt=Mandatory`r`n" }
    if (![regex]::IsMatch($body,'(?m)^TrustServerCertificate=')) { $body = $body.TrimEnd()+"`r`nTrustServerCertificate=false`r`n" }
    $updated = $updated.Substring(0,$match.Index)+$match.Groups[1].Value+$body+$updated.Substring($match.Index+$match.Length)
}
if ($updated -eq $content) { Write-Output "Already uses direct SQL Server settings: $Path"; return }
$backup = $Path+'.before-direct-sql-'+[DateTime]::Now.ToString('yyyyMMdd-HHmmss')+'-'+[guid]::NewGuid().ToString('N')+'.bak'
$temporary = $Path+'.'+[guid]::NewGuid().ToString('N')+'.tmp'
try {
    [IO.File]::WriteAllText($temporary,$updated,[Text.UTF8Encoding]::new($false))
    [IO.File]::Replace($temporary,$Path,$backup)
} finally { if (Test-Path -LiteralPath $temporary) { Remove-Item -LiteralPath $temporary } }
Write-Output "Migrated $Path; previous configuration retained at $backup"
