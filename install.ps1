# Registers a right-click context menu entry on image files.
# Safe to re-run, re-registering overwrites the same key.

$ErrorActionPreference = 'Stop'

$menuLabel = "Open image as a web screenshot"
$menuId    = "OpenAsWebScreenshot"


$executable  = Join-Path $PSScriptRoot "/bin/launcher.exe"
$viewer = Join-Path $PSScriptRoot "/html/view.html"

if (-not (Test-Path $executable) || (Test-Path $viewer)) {
    Write-Error "Could not find launcher.ps1 next to this script. Aborting."
    exit 1
}

$keyPath = "HKCU:\Software\Classes\SystemFileAssociations\image\shell\$menuId"
$cmdPath = "$keyPath\command"

New-Item -Path $keyPath -Force | Out-Null
Set-ItemProperty -Path $keyPath -Name "(default)" -Value $menuLabel

# Optionally to assign an icon later:
# Set-ItemProperty -Path $keyPath -Name "Icon" -Value (Join-Path $PSScriptRoot "icon.ico")

New-Item -Path $cmdPath -Force | Out-Null
$commandValue = "`"$executable`" `"$viewer`" `"%1`""
Set-ItemProperty -Path $cmdPath -Name "(default)" -Value $commandValue

Write-Host "Installed: '$menuLabel' as a context menu entry."