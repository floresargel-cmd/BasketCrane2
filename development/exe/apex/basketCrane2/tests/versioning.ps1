$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot '..\installer\Versioning.ps1')
$cases = @{
    '2.5.8'='2.5.9'; '2.5.98'='2.5.99'; '2.5.99'='2.6.0';
    '2.98.99'='2.99.0'; '2.99.98'='2.99.99'; '2.99.99'='3.0.0'
}
foreach ($value in $cases.Keys) {
    if ((Get-NextBasketVersion $value).ToString(3) -ne $cases[$value]) { throw "Rollover failed: $value" }
}
foreach ($invalid in @('2.100.0','2.5.100','2.-1.0','2.5','2.5.8.0','256.0.0','02.5.8')) {
    $rejected = $false
    try { ConvertTo-BasketVersion $invalid | Out-Null } catch { $rejected = $true }
    if (!$rejected) { throw "Invalid version accepted: $invalid" }
}
$rejected = $false
try { Get-NextBasketVersion '255.99.99' | Out-Null } catch { $rejected = $true }
if (!$rejected) { throw 'MSI version overflow accepted.' }
$tempHeader = Join-Path ([IO.Path]::GetTempPath()) ('basket-version-' + [guid]::NewGuid() + '.h')
try {
    Copy-Item -LiteralPath (Join-Path $PSScriptRoot '..\version.h') -Destination $tempHeader
    $original = Get-BasketVersion $tempHeader
    & (Join-Path $PSScriptRoot '..\installer\Set-Version.ps1') -Next -Header $tempHeader | Out-Null
    if ((Get-BasketVersion $tempHeader) -ne (Get-NextBasketVersion $original.ToString(3))) { throw 'Header update failed.' }
} finally { Remove-Item -LiteralPath $tempHeader -ErrorAction SilentlyContinue }
Write-Output 'PASS: version validation, header synchronization and both 99 rollovers.'
