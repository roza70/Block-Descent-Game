#!/usr/bin/env bash
# Local "make" for Git Bash when GNU make is not installed.
# Usage from project folder:  ./make   then   ./sai_blocks.exe
set -e
cd "$(dirname "$0")"

for MINGW in /c/msys64/mingw64 /c/Users/Roza/Downloads/mingw64 "$HOME/Downloads/mingw64"; do
  [ -x "$MINGW/bin/g++.exe" ] && break
done
if [ ! -x "$MINGW/bin/g++.exe" ]; then
  echo "MinGW g++ not found. Install MSYS2 or set MINGW=/path/to/mingw64"
  exit 1
fi

export PATH="$MINGW/bin:$PATH"
GXX="$MINGW/bin/g++"
CXXFLAGS="-std=c++17 -Wall -O2 -Isrc -I$MINGW/include/SDL2"
LDFLAGS="-L$MINGW/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows"

cmd="${1:-all}"
case "$cmd" in
  clean)
    rm -f src/*.o sai_blocks.exe
    echo "Cleaned."
    ;;
  all)
    for f in src/main.cpp src/game.cpp src/renderer.cpp src/input.cpp src/particles.cpp; do
      o="${f%.cpp}.o"
      "$GXX" $CXXFLAGS -c -o "$o" "$f"
    done
    "$GXX" -o sai_blocks.exe src/*.o $LDFLAGS
    for d in SDL2.dll SDL2_ttf.dll libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll; do
      cp -f "$MINGW/bin/$d" . 2>/dev/null || true
    done
    echo "Built sai_blocks.exe"
    ;;
  *)
    echo "Usage: ./make [all|clean]"
    exit 1
    ;;
esac
