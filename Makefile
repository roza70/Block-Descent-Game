# Git Bash:  export PATH="/c/msys64/mingw64/bin:$PATH"   (or your MinGW bin)
#           make
#           ./sai_blocks.exe
#
# No make installed? Use:  ./make

MINGW   ?= /c/msys64/mingw64
ifeq ($(wildcard $(MINGW)/bin/g++.exe),)
  MINGW := /c/Users/Roza/Downloads/mingw64
endif

CXX      = $(MINGW)/bin/g++
CXXFLAGS = -std=c++17 -Wall -O2 -Isrc -I$(MINGW)/include/SDL2
LDFLAGS  = -L$(MINGW)/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -mwindows

SRC = src/main.cpp src/game.cpp src/renderer.cpp \
      src/input.cpp src/particles.cpp
OBJ = $(SRC:.cpp=.o)
BIN = sai_blocks.exe

DLLS = SDL2.dll SDL2_ttf.dll libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll

.PHONY: all clean run dlls

all: dlls $(BIN)

$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

dlls:
	@for d in $(DLLS); do \
		cp -f "$(MINGW)/bin/$$d" . 2>/dev/null || true; \
	done

run: all
	./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)
