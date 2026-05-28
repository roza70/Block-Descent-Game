CXX = g++

CXXFLAGS = -std=c++17 -Wall -O2 -Isrc -IC:/msys64/mingw64/include/SDL2

LDFLAGS = -LC:/msys64/mingw64/lib -lSDL2 -lSDL2_ttf -lSDL2main -mwindows

SRC = src/main.cpp src/game.cpp src/renderer.cpp src/input.cpp src/particles.cpp

OBJ = $(SRC:.cpp=.o)

BIN = sai_blocks.exe