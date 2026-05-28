#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "particles.h"

const int WIN_W = 700;
const int WIN_H = ROWS * BLOCK_SIZE + 80;
const int FPS = 60;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *win = SDL_CreateWindow(
        "Sai Nanami - Block Descent",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_W, WIN_H, SDL_WINDOW_SHOWN);

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
                                           SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

    const char *fontPaths[] = {
        "C:/Windows/Fonts/arialbd.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/verdana.ttf",
        nullptr};

    TTF_Font *font = nullptr;
    TTF_Font *smallFont = nullptr;
    for (int i = 0; fontPaths[i] && !font; i++)
    {
        font = TTF_OpenFont(fontPaths[i], 22);
        smallFont = TTF_OpenFont(fontPaths[i], 13);
    }

    Game *game = new Game();
    Renderer renderer(ren, font, smallFont);
    ParticleSystem particles;

    const Uint32 frameMs = 1000 / FPS;
    Uint32 last = SDL_GetTicks();

    while (true)
    {
        Input in = handleInput();
        if (in.quit)
            break;
        if (in.restart)
        {
            delete game;
            game = new Game();
        }

        if (!game->isOver() && !game->isFlashing())
        {
            if (in.left)
                game->moveLeft();
            if (in.right)
                game->moveRight();
            if (in.down)
                game->moveDown();
            if (in.rotate)
                game->rotate();
            if (in.hardDrop)
                game->hardDrop();
        }

        game->update();
        renderer.drawAll(*game, particles);

        Uint32 elapsed = SDL_GetTicks() - last;
        if (elapsed < frameMs)
            SDL_Delay(frameMs - elapsed);
        last = SDL_GetTicks();
    }

    delete game;
    if (font)
        TTF_CloseFont(font);
    if (smallFont)
        TTF_CloseFont(smallFont);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}