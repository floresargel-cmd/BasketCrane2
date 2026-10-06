[CmdletBinding(DefaultParameterSetName='Next')]
param(
    [Parameter(ParameterSetName='Next')][switch]$Next,
    [Parameter(Mandatory=$true,ParameterSetName='Explicit')][string]$Version,
    [string]$Header = (Join-Path $PSScriptRoot '..\version.h')
)
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Versioning.ps1')
$current = Get-BasketVersion $Header
$target = if ($PSCmdlet.ParameterSetName -eq 'Explicit') { ConvertTo-BasketVersion $Version } else { Get-NextBasketVersion ($current.ToString(3)) }
if ($target -le $current) { throw 'A release version must increase.' }
$content = @"
#pragma once
// Application version; independent of the Qt 5.7.0 toolchain version.
// Update with installer/Set-Version.ps1 so metadata stays synchronized.
#define BASKET_VERSION_MAJOR $($target.Major)
#define BASKET_VERSION_MINOR $($target.Minor)
#define BASKET_VERSION_PATCH $($target.Build)
#define BASKET_VERSION_STRING "$($target.ToString(3))"
"@
[IO.File]::WriteAllText($Header, $content + "`r`n", [Text.UTF8Encoding]::new($false))
Write-Output "$($current.ToString(3)) -> $($target.ToString(3))"
