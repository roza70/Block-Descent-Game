#!/usr/bin/env bash
cd "$(dirname "$0")"
export PATH="/c/msys64/mingw64/bin:$PATH"
MINGW="/c/msys64/mingw64/bin"
for dll in SDL2.dll SDL2_ttf.dll libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll; do
  [ -f "$MINGW/$dll" ] && cp -f "$MINGW/$dll" . 2>/dev/null
done
if [ ! -f sai_blocks.exe ] || [ src/main.cpp -nt sai_blocks.exe ]; then
  make -s || { echo "Build failed. Need: MSYS2 MinGW g++ and SDL2."; exit 1; }
fi
exec ./sai_blocks.exe
