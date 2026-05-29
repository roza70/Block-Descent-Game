#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "particles.h"

const int WIN_W = 700;
const int WIN_H = ROWS * BLOCK_SIZE + 80;
const int FPS = 60;

static const char *HIT_QUOTES[] = {
    "Star bump!", "Watch out!", "Too close!",
    "Focus!", "Almost!", "Whoa!"};

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
        "C:/Windows/Fonts/consolab.ttf",
        "C:/Windows/Fonts/consola.ttf",
        "C:/Windows/Fonts/verdana.ttf",
        nullptr};
    TTF_Font *font = nullptr;
    TTF_Font *sm = nullptr;
    for (int i = 0; fontPaths[i] && !font; i++)
    {
        font = TTF_OpenFont(fontPaths[i], 22);
        sm = TTF_OpenFont(fontPaths[i], 13);
    }

    Game *game = new Game();
    Renderer renderer(ren, font, sm);
    ParticleSystem particles;
    StarField stars;
    ScorePopups scorePopups;
    FallingHazards hazards;

    renderer.triggerQuote("Ready!");

    int lastScore = 0;
    int lastLevel = 1;
    int lastLines = 0;
    int quoteTimer = 0;

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
            hazards.reset();
            renderer.triggerQuote("Again!");
            lastScore = 0;
            lastLevel = 1;
            lastLines = 0;
            quoteTimer = 0;
        }

        if (!game->isOver() && !game->isStunned())
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
            {
                game->hardDrop();
                renderer.triggerQuote("Hard drop!");
                quoteTimer = 90;
            }
        }

        game->update();

        hazards.update(*game, particles);
        if (hazards.checkPieceHit(*game, particles))
        {
            game->applyHazardHit();
            renderer.triggerQuote(HIT_QUOTES[rand() % 6]);
            quoteTimer = 80;
        }

        if (game->newQuote && quoteTimer <= 0)
        {
            renderer.triggerQuote(game->currentQuote);
            quoteTimer = 90;
        }
        if (quoteTimer > 0)
            quoteTimer--;

        if (game->getLevel() != lastLevel)
        {
            renderer.triggerQuote("Level " + std::to_string(game->getLevel()) + "!");
            lastLevel = game->getLevel();
            quoteTimer = 90;
        }

        int sc = game->getScore();
        if (quoteTimer <= 0)
        {
            if (sc >= 5000 && lastScore < 5000)
                renderer.triggerQuote("5000! Insane!");
            else if (sc >= 3000 && lastScore < 3000)
                renderer.triggerQuote("3000! Nice!");
            else if (sc >= 1000 && lastScore < 1000)
                renderer.triggerQuote("1000 points!");
            else if (sc >= 500 && lastScore < 500)
                renderer.triggerQuote("500!");
        }

        int lines = game->getLinesCleared();
        if (lines != lastLines)
        {
            int cleared = lines - lastLines;
            int pts[] = {0, 100, 300, 500, 800};
            std::string popup;
            SDL_Color popColor = {255, 255, 255, 255};

            if (cleared == 1)
            {
                popup = "Line!";
                popColor = {255, 200, 0, 255};
            }
            else if (cleared == 2)
            {
                popup = "Double!";
                popColor = {100, 200, 255, 255};
            }
            else if (cleared == 3)
            {
                popup = "Triple!";
                popColor = {200, 100, 255, 255};
            }
            else if (cleared >= 4)
            {
                popup = "TETRIS!";
                popColor = {255, 100, 100, 255};
            }

            if (!popup.empty())
            {
                scorePopups.emit(400, 200, popup, popColor);
                renderer.triggerQuote(cleared >= 4 ? "TETRIS!!" : "Nice clear!");
                renderer.mindBurst();
            }

            scorePopups.emit(400, 240,
                             "+" + std::to_string(pts[std::min(cleared, 4)] * game->getLevel()),
                             {100, 255, 150, 255});
            lastLines = lines;
        }

        lastScore = sc;
        renderer.drawAll(*game, particles, stars, scorePopups, hazards);

        Uint32 elapsed = SDL_GetTicks() - last;
        if (elapsed < frameMs)
            SDL_Delay(frameMs - elapsed);
        last = SDL_GetTicks();
    }

    delete game;
    if (font)
        TTF_CloseFont(font);
    if (sm)
        TTF_CloseFont(sm);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
