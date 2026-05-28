#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "particles.h"
#include <string>

const SDL_Color PIECE_COLORS[PIECE_COUNT] = {
    {0, 255, 200, 255},
    {255, 220, 50, 255},
    {180, 100, 255, 255},
    {50, 200, 255, 255},
    {255, 100, 100, 255},
    {100, 255, 100, 255},
    {255, 160, 50, 255},
};

const char *const PIECE_NAMES[PIECE_COUNT] = {
    "ALGORITHM", "BINARY", "FUNCTION",
    "ARRAY", "POINTER", "LOOP", "STACK"};

class Renderer
{
public:
    Renderer(SDL_Renderer *r, TTF_Font *font, TTF_Font *smallFont);
    void drawAll(const Game &g, ParticleSystem &ps);

private:
    SDL_Renderer *ren;
    TTF_Font *font;
    TTF_Font *smallFont;

    void drawBackground();
    void drawGrid(const Game &g, ParticleSystem &ps);
    void drawPiece(const Piece &p, int ox, int oy);
    void drawGhost(const Game &g);
    void drawSidePanel(const Game &g);
    void drawBlock(int x, int y, SDL_Color col);
    void drawText(const std::string &t, int x, int y,
                  SDL_Color col, TTF_Font *f = nullptr);
};