param([Parameter(Mandatory=$true)][string]$Executable)
$ErrorActionPreference='Stop'
# Inspect the PE import directory directly; no extra SDK tools and no execution.
$bytes=[IO.File]::ReadAllBytes((Resolve-Path -LiteralPath $Executable).Path)
function U16([int]$offset) { [BitConverter]::ToUInt16($bytes,$offset) }
function U32([int]$offset) { [BitConverter]::ToUInt32($bytes,$offset) }
$pe=[int](U32 0x3c)
if ((U16 0) -ne 0x5a4d -or (U32 $pe) -ne 0x4550) { throw 'Invalid PE executable.' }
$optional=$pe+24
$magic=U16 $optional
if ($magic -ne 0x10b -and $magic -ne 0x20b) { throw 'Unsupported PE format.' }
$directory=$optional+$(if ($magic -eq 0x10b) {96} else {112})
$importRva=U32 ($directory+8)
$sectionStart=$optional+(U16 ($pe+20))
$sections=@()
for ($i=0;$i -lt (U16 ($pe+6));$i++) {
    $offset=$sectionStart+40*$i
    $sections+=@{Rva=(U32 ($offset+12));Size=[Math]::Max((U32 ($offset+8)),(U32 ($offset+16)));Raw=(U32 ($offset+20))}
}
function FileOffset([uint32]$rva) {
    if ($rva -lt (U32 ($optional+60))) { return [int]$rva }
    foreach ($section in $sections) { if ($rva -ge $section.Rva -and $rva -lt $section.Rva+$section.Size) { return [int]($section.Raw+$rva-$section.Rva) } }
    throw 'PE import RVA is outside mapped sections.'
}
$imports=@()
if ($importRva) {
    $descriptor=FileOffset $importRva
    while ((U32 ($descriptor+12)) -ne 0) {
        $start=FileOffset (U32 ($descriptor+12)); $end=$start
        while ($bytes[$end] -ne 0 -and $end -lt $bytes.Length) { $end++ }
        $imports += [Text.Encoding]::ASCII.GetString($bytes,$start,$end-$start)
        $descriptor+=20
    }
}
if (@($imports | Where-Object { $_ -match '^odbc' }).Count) { throw 'ODBC DLL dependency remains in executable.' }
Write-Output 'PASS: executable imports contain no ODBC DLL dependency.'
