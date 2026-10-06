param([Parameter(Mandatory=$true)][string]$Msi)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot '..\installer\Versioning.ps1')
$version = (Get-BasketVersion (Join-Path $PSScriptRoot '..\version.h')).ToString(3)
# Read MSI tables only: this check neither installs nor launches the application.
$engine = New-Object -ComObject WindowsInstaller.Installer
$database = $engine.OpenDatabase((Resolve-Path -LiteralPath $Msi).Path,0)
function Get-MsiRows([string]$Sql,[int]$Columns) {
    $view = $database.OpenView($Sql)
    try {
        [void]$view.Execute()
        while ($record = $view.Fetch()) {
            $row = @()
            for ($column=1; $column -le $Columns; $column++) { $row += $record.StringData($column) }
            ,$row
        }
    } finally { [void]$view.Close() }
}
$properties = @{}
foreach ($row in (Get-MsiRows 'SELECT `Property`, `Value` FROM `Property`' 2)) { $properties[$row[0]]=$row[1] }
if ($properties['ProductVersion'] -ne $version) { throw 'MSI version disagrees with version.h.' }
if ($properties['UpgradeCode'] -ne '{F30F74DC-F339-4C63-9D88-7A484BF10E5D}') { throw 'Upgrade identity changed.' }
if ($properties['ALLUSERS'] -ne '1') { throw 'MSI must use per-machine installation.' }
if (@(Get-MsiRows 'SELECT `Property` FROM `AppSearch`' 1 | Where-Object { $_[0] -eq 'SQLSERVERCLIENT' }).Count -ne 1) { throw 'Native SQL Server client detection missing.' }
$conditions=@(Get-MsiRows 'SELECT `Condition` FROM `LaunchCondition`' 1)
if (@($conditions | Where-Object { $_[0] -like '*SQLSERVERCLIENT*' }).Count -ne 1) { throw 'Native SQL Server prerequisite missing.' }
$components = @(Get-MsiRows 'SELECT `Component`, `Attributes` FROM `Component`' 2)
$configuration = $components | Where-Object { $_[0] -eq 'SiteConfiguration' }
if (!$configuration -or (([int]$configuration[1] -band 144) -ne 144)) { throw 'Configuration must be permanent and never overwritten.' }
$files = @(Get-MsiRows 'SELECT `File`, `FileName` FROM `File`' 2)
if (@($files | Where-Object { $_[0] -eq 'MainExecutable' }).Count -ne 1) { throw 'Application EXE missing.' }
$inis = @($files | Where-Object { $_[1] -match '\.ini$' })
if ($inis.Count -ne 1 -or $inis[0][0] -ne 'ConfigurationFile') { throw 'Only the configuration template may be packaged.' }
if (@($files | Where-Object { $_[1] -match '\.(pdb|lib|obj)$' }).Count -gt 0) { throw 'Build intermediates leaked into the installer.' }
$actions = @(Get-MsiRows 'SELECT `Action`, `Sequence` FROM `InstallExecuteSequence`' 2)
$remove = $actions | Where-Object { $_[0] -eq 'RemoveExistingProducts' }
$initialize = $actions | Where-Object { $_[0] -eq 'InstallInitialize' }
if (!$remove -or !$initialize -or [int]$remove[1] -le [int]$initialize[1]) { throw 'Upgrade removal must participate in rollback.' }
if (@(Get-MsiRows 'SELECT `UpgradeCode` FROM `Upgrade`' 1).Count -lt 2) { throw 'Upgrade and downgrade detection missing.' }
$shortcuts = @(Get-MsiRows 'SELECT `Arguments` FROM `Shortcut`' 1)
if (@($shortcuts | Where-Object { $_[0] -like '*[[]CONFIGFOLDER[]]basketCrane2.ini*' }).Count -ne 1) { throw 'Start Menu configuration path missing.' }
Write-Output "PASS: MSI $version metadata, upgrade identity, rollback sequence, persistent configuration, shortcuts and payload allowlist."
