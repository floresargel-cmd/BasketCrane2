[CmdletBinding()]
param(
    [string]$MSBuild,
    [string]$Wix,
    [string]$BuildDirectory = (Join-Path $PSScriptRoot 'out'),
    [switch]$SkipApplicationBuild,
    [switch]$SkipVersionIncrement
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Versioning.ps1')
$projectRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$BuildDirectory = [IO.Path]::GetFullPath($BuildDirectory)
$releaseDir = Join-Path $BuildDirectory 'release'
New-Item -ItemType Directory -Force -Path $releaseDir | Out-Null
if (!$Wix) {
    $wixCommand = Get-Command wix -ErrorAction SilentlyContinue
    if ($wixCommand) { $Wix = $wixCommand.Source }
    else {
        foreach ($candidate in @(
            (Join-Path ([Environment]::GetFolderPath('LocalApplicationData')) 'Apex\build-tools\wix4\wix.exe'),
            (Join-Path $env:LOCALAPPDATA 'Apex\build-tools\wix4\wix.exe'),
            (Join-Path $env:USERPROFILE '.dotnet\tools\wix.exe'),
            'C:\build-tools\wix4\wix.exe'
        )) {
            if (Test-Path -LiteralPath $candidate) { $Wix = $candidate; break }
        }
    }
    if (!$Wix) { throw "WiX is required. Tool lookup profile: $env:USERPROFILE; LocalAppData: $env:LOCALAPPDATA; Windows LocalAppData: $([Environment]::GetFolderPath('LocalApplicationData')). See installer\README.txt or supply -Wix." }
}
Write-Output "WiX CLI: $Wix"
if (!$SkipApplicationBuild) {
    if (!$MSBuild) {
        $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
        $MSBuild = & $vswhere -latest -products '*' -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
    }
    if (!$MSBuild -or !(Test-Path -LiteralPath $MSBuild)) { throw 'MSBuild with the C++ desktop toolset is required.' }
    # Build only the application: building the solution would invoke this installer recursively.
    Write-Output 'Building Release application...'
    $skipIncrement = if ($SkipVersionIncrement) { 'true' } else { 'false' }
    & $MSBuild (Join-Path $projectRoot 'sln\apexBasketCrane2.5.7.0.vcxproj') /m /p:Configuration=Release /p:Platform=Win32 "/p:SkipBasketVersionIncrement=$skipIncrement" "/p:OutDir=$releaseDir/" "/p:IntDir=$BuildDirectory/obj/Release/" /v:minimal /nologo
    if ($LASTEXITCODE -ne 0) { throw 'Release application build failed.' }
}
$version = (Get-BasketVersion (Join-Path $projectRoot 'version.h')).ToString(3)
$payloadDir = Join-Path $BuildDirectory ('payload-' + $version + '-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Force -Path $payloadDir | Out-Null
$executable = Join-Path $releaseDir 'apexBasketCrane2.exe'
if (!(Test-Path -LiteralPath $executable)) { throw "Release executable missing: $executable" }
$metadata = [Diagnostics.FileVersionInfo]::GetVersionInfo($executable)
if ($metadata.ProductVersion -ne $version -or $metadata.FileVersion -ne $version) {
    throw "Executable metadata must match version.h ($version). Rebuild before packaging."
}
& (Join-Path $projectRoot 'tests\versioning.ps1')
& (Join-Path $projectRoot 'tests\configurationMigration.ps1')
& (Join-Path $projectRoot 'tests\nativeTransport.ps1') -Executable $executable
$testConfiguration = Join-Path $releaseDir 'basketCrane2.ini'
# The application build seeds this isolated output with the local site's INI.
# Always stage the shipped template for offline checks, including default-path lookup.
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'basketCrane2.template.ini') -Destination $testConfiguration -Force
& (Join-Path $projectRoot 'tests\configuration.ps1') -Executable $executable
Copy-Item -LiteralPath $executable -Destination $payloadDir
foreach ($file in @('CONFIGURATION.txt','ARCHITECTURE.txt','version.h')) {
    Copy-Item -LiteralPath (Join-Path $projectRoot $file) -Destination $payloadDir
}
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'README.txt') -Destination (Join-Path $payloadDir 'INSTALLATION.txt')
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'TaskbarLayout.xml') -Destination $payloadDir
Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'Migrate-Configuration.ps1') -Destination $payloadDir
$vlcDir = [IO.Path]::GetFullPath((Join-Path $projectRoot '..\..\..\libs\_deploy\builds\vlc\vlc-2.2.1'))
foreach ($file in @('libvlc.dll','libvlccore.dll')) {
    Copy-Item -LiteralPath (Join-Path $vlcDir $file) -Destination $payloadDir
}
foreach ($folder in @('plugins','lua','locale')) {
    Copy-Item -LiteralPath (Join-Path $vlcDir $folder) -Destination (Join-Path $payloadDir $folder) -Recurse
}
$licenseDir = Join-Path $payloadDir 'licenses\VLC'
New-Item -ItemType Directory -Force -Path $licenseDir | Out-Null
foreach ($file in @('COPYING.txt','AUTHORS.txt','README.txt')) {
    Copy-Item -LiteralPath (Join-Path $vlcDir $file) -Destination $licenseDir
}
# Explicit allowlist above excludes the site's INI, build intermediates and PDBs.
# Harvest one file per component. Stable destination paths generate stable GUIDs
# for upgrades; source staging paths are allowed to vary between releases.
$namespace = 'http://wixtoolset.org/schemas/v4/wxs'
$document = [xml]"<Wix xmlns='$namespace'><Fragment><ComponentGroup Id='RuntimePayload' /></Fragment></Wix>"
$group = $document.DocumentElement.FirstChild.FirstChild
function Add-XmlElement($Parent, [string]$Name, [hashtable]$Attributes) {
    $element = $document.CreateElement($Name,$namespace)
    foreach ($key in $Attributes.Keys) { $element.SetAttribute($key,[string]$Attributes[$key]) }
    [void]$Parent.AppendChild($element)
    return $element
}
$directories = @{''='INSTALLFOLDER'}
$counter = 0
foreach ($file in (Get-ChildItem -LiteralPath $payloadDir -Recurse -File | Sort-Object FullName)) {
    if ($file.Name -eq 'apexBasketCrane2.exe') { continue }
    $relative = $file.FullName.Substring($payloadDir.Length + 1)
    $relativeDirectory = [IO.Path]::GetDirectoryName($relative)
    if (!$directories.ContainsKey($relativeDirectory)) {
        $parentPath = ''; $parentId = 'INSTALLFOLDER'
        foreach ($part in $relativeDirectory.Split('\')) {
            $parentPath = if ($parentPath) { $parentPath + '\' + $part } else { $part }
            if (!$directories.ContainsKey($parentPath)) {
                $counter++; $id = 'RuntimeDir' + $counter
                $directoryRef = Add-XmlElement $document.DocumentElement.FirstChild 'DirectoryRef' @{Id=$parentId}
                Add-XmlElement $directoryRef 'Directory' @{Id=$id;Name=$part} | Out-Null
                $directories[$parentPath] = $id
            }
            $parentId = $directories[$parentPath]
        }
    }
    $counter++
    $component = Add-XmlElement $group 'Component' @{Id=('RuntimeComponent'+$counter);Guid='*';Directory=$directories[$relativeDirectory]}
    Add-XmlElement $component 'File' @{Id=('RuntimeFile'+$counter);Source=$file.FullName;KeyPath='yes'} | Out-Null
}
$harvest = Join-Path $BuildDirectory 'RuntimePayload.wxs'
$document.Save($harvest)
$output = Join-Path $BuildDirectory "ApexBasketCrane2-$version-x86.msi"
Write-Output "Packaging MSI: $output"
& $Wix build (Join-Path $PSScriptRoot 'Package.wxs') $harvest -arch x86 -ext WixToolset.UI.wixext `
    -d "AppVersion=$version" -d "ProjectRoot=$projectRoot" -d "PayloadDir=$payloadDir" -out $output -pdbtype none
if ($LASTEXITCODE -ne 0) { throw 'MSI compilation or validation failed.' }
& (Join-Path $projectRoot 'tests\installer.ps1') -Msi $output
$hashAlgorithm = [Security.Cryptography.SHA256]::Create()
$hashStream = [IO.File]::OpenRead($output)
try {
    $hash = [BitConverter]::ToString($hashAlgorithm.ComputeHash($hashStream)).Replace('-','')
    Write-Output "SHA256: $hash"
} finally {
    $hashStream.Dispose()
    $hashAlgorithm.Dispose()
}
Write-Output "Installer created: $output"
