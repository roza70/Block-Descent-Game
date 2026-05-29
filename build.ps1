# Build script for Block Descent Game

$CXX = "g++"
$CXXFLAGS = @("-std=c++17", "-Wall", "-O2", "-Isrc", "-IC:/msys64/mingw64/include/SDL2")
$LDFLAGS = @("-LC:/msys64/mingw64/lib", "-lmingw32", "-lSDL2main", "-lSDL2", "-lSDL2_ttf")
$SRC = @("src/main.cpp", "src/game.cpp", "src/renderer.cpp", "src/input.cpp", "src/particles.cpp")
$BIN = "sai_blocks.exe"

# Compile object files
Write-Host "Compiling..."
foreach ($file in $SRC) {
    $obj = $file -replace "\.cpp$", ".o"
    & $CXX @CXXFLAGS -c -o $obj $file
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Compilation failed for $file"
        exit 1
    }
}

# Link
Write-Host "Linking..."
$OBJ = $SRC | ForEach-Object { $_ -replace "\.cpp$", ".o" }
& $CXX -o $BIN $OBJ @LDFLAGS

if ($LASTEXITCODE -eq 0) {
    $Mingw = "C:\msys64\mingw64\bin"
    foreach ($dll in @("SDL2.dll", "SDL2_ttf.dll", "libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll")) {
        $src = Join-Path $Mingw $dll
        if (Test-Path $src) { Copy-Item $src $PSScriptRoot -Force }
    }
    Write-Host "Build successful! Run with: .\run.ps1"
} else {
    Write-Error "Linking failed"
    exit 1
}
