# Run Block Descent — copies SDL DLLs next to the exe if needed
$ErrorActionPreference = "Stop"
$Root = $PSScriptRoot
$Mingw = "C:\msys64\mingw64\bin"

if (-not (Test-Path "$Root\sai_blocks.exe")) {
    Write-Host "Building first..."
    & "$Root\build.ps1"
}

$dlls = @("SDL2.dll", "SDL2_ttf.dll", "libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll")
foreach ($dll in $dlls) {
    $src = Join-Path $Mingw $dll
    if (Test-Path $src) {
        Copy-Item $src $Root -Force
    }
}

$env:PATH = "$Mingw;$Root;$env:PATH"
Set-Location $Root
Write-Host "Starting Sai Nanami - Block Descent..."
& ".\sai_blocks.exe"
