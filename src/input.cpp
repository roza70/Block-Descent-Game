#include "input.h"

Input handleInput()
{
    Input in;
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            in.quit = true;
        if (e.type == SDL_KEYDOWN)
            switch (e.key.keysym.sym)
            {
            case SDLK_LEFT:
                in.left = true;
                break;
            case SDLK_RIGHT:
                in.right = true;
                break;
            case SDLK_DOWN:
                in.down = true;
                break;
            case SDLK_UP:
                in.rotate = true;
                break;
            case SDLK_SPACE:
                in.hardDrop = true;
                break;
            case SDLK_r:
                in.restart = true;
                break;
            case SDLK_q:
                in.quit = true;
                break;
            }
    }
    return in;
}