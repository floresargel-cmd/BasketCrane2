$ErrorActionPreference='Stop'
$folder=Join-Path ([IO.Path]::GetTempPath()) ('basket-migration-'+[guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $folder | Out-Null
try {
    $path=Join-Path $folder 'fixture.ini'
    $legacy=[IO.File]::ReadAllText((Join-Path $PSScriptRoot '..\installer\basketCrane2.template.ini'))
    $legacy=$legacy.Replace('Server=CHANGE_ME_SQL_SERVER','Dsn=missing-'+[guid]::NewGuid().ToString('N'))
    $legacy=[regex]::Replace($legacy,'(?m)^(Encrypt|TrustServerCertificate)=[^\r\n]*\r?\n','')
    [IO.File]::WriteAllText($path,$legacy)
    $refused=$false
    try { & (Join-Path $PSScriptRoot '..\installer\Migrate-Configuration.ps1') -Path $path } catch { $refused=$true }
    if (!$refused -or [IO.File]::ReadAllText($path) -ne $legacy) { throw 'Unresolved migration must leave configuration unchanged.' }
    & (Join-Path $PSScriptRoot '..\installer\Migrate-Configuration.ps1') -Path $path -Servers @{BasketDatabase='host\instance';EpicsDatabase='epics,1433';Epics2Database='epics2,1444'}
    $migrated=[IO.File]::ReadAllText($path)
    if ($migrated -match '(?m)^Dsn=' -or !$migrated.Contains('Server=host\\instance') -or !$migrated.Contains('Server=epics,1433') -or !$migrated.Contains('Mode=LiveObserver') -or !$migrated.Contains('Password=CHANGE_ME') -or [regex]::Matches($migrated,'(?m)^Encrypt=Mandatory').Count -ne 3) { throw 'Migrated endpoints/settings are incorrect.' }
    $backups=@(Get-ChildItem -LiteralPath $folder -File | Where-Object { $_.Extension -eq '.bak' })
    if ($backups.Count -ne 1 -or [IO.File]::ReadAllText($backups[0].FullName) -ne $legacy) { throw 'Backup must preserve original settings exactly.' }
    & (Join-Path $PSScriptRoot '..\installer\Migrate-Configuration.ps1') -Path $path
    if ([IO.File]::ReadAllText($path) -ne $migrated) { throw 'Migration must be idempotent.' }
    Write-Output 'PASS: configuration migration preserves credentials/mode, resolves endpoints, backs up atomically, refuses unresolved DSNs and is idempotent.'
} finally {
    $resolved=[IO.Path]::GetFullPath($folder)
    if ([IO.Path]::GetDirectoryName($resolved) -ne [IO.Path]::GetTempPath().TrimEnd('\') -or ![IO.Path]::GetFileName($resolved).StartsWith('basket-migration-')) { throw 'Unexpected cleanup path' }
    foreach ($file in (Get-ChildItem -LiteralPath $resolved -File)) { Remove-Item -LiteralPath $file.FullName }
    Remove-Item -LiteralPath $resolved
}
