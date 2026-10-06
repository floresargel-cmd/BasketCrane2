Set-StrictMode -Version Latest
function ConvertTo-BasketVersion([string]$Value) {
    if ($Value -notmatch '^(0|[1-9][0-9]*)\.(0|[1-9][0-9]?)\.(0|[1-9][0-9]?)$') {
        throw 'Version must be major.minor.patch, with minor and patch from 0 to 99.'
    }
    $parts = $Value.Split('.')
    $major = [int]$parts[0]
    if ($major -gt 255) { throw 'Windows MSI supports a major version up to 255.' }
    return [version]::new($major, [int]$parts[1], [int]$parts[2])
}
function Get-BasketVersion([string]$Header) {
    $content = [IO.File]::ReadAllText($Header)
    $parts = foreach ($field in @('MAJOR','MINOR','PATCH')) {
        $match = [regex]::Match($content, '(?m)^#define BASKET_VERSION_' + $field + '\s+(\d+)\s*$')
        if (!$match.Success) { throw "Missing version field: $field" }
        $match.Groups[1].Value
    }
    $value = ConvertTo-BasketVersion ($parts -join '.')
    if ($content -notmatch ('#define BASKET_VERSION_STRING "' + [regex]::Escape($value.ToString(3)) + '"')) {
        throw 'Version string and numeric metadata disagree.'
    }
    return $value
}
function Get-NextBasketVersion([string]$Value) {
    $current = ConvertTo-BasketVersion $Value
    $major = $current.Major; $minor = $current.Minor; $patch = $current.Build + 1
    if ($patch -gt 99) { $patch = 0; ++$minor }
    if ($minor -gt 99) { $minor = 0; ++$major }
    return ConvertTo-BasketVersion "$major.$minor.$patch"
}
