#pragma once
#include <SDL2/SDL.h>

struct Input
{
    bool quit = false;
    bool left = false;
    bool right = false;
    bool down = false;
    bool rotate = false;
    bool hardDrop = false;
    bool restart = false;
};

Input handleInput();