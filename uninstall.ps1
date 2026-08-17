# Removes the context menu entry created by install.ps1

$menuId  = "OpenAsWebScreenshot"
$keyPath = "HKCU:\Software\Classes\SystemFileAssociations\image\shell\$menuId"

if (Test-Path $keyPath) {
    Remove-Item -Path $keyPath -Recurse -Force
    Write-Host "Removed context menu entry."
} else {
    Write-Host "Nothing to remove (entry not found)."
}