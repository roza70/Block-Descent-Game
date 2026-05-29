#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "particles.h"
#include "sprite.h"
#include <string>
#include <vector>

const SDL_Color PIECE_COLORS[PIECE_COUNT] = {
    {0, 255, 220, 255},
    {255, 200, 0, 255},
    {255, 80, 180, 255},
    {0, 200, 255, 255},
    {255, 70, 70, 255},
    {60, 255, 120, 255},
    {255, 140, 0, 255},
};

const char *const PIECE_NAMES[PIECE_COUNT] = {
    "ALGORITHM", "BINARY", "FUNCTION",
    "ARRAY", "POINTER", "LOOP", "STACK"};

struct HeadQuote
{
    std::string text;
    float x, y, vy;
    int timer, maxTimer;
    float alpha;
};

const int GX = 158;
const int GY = 40;

class Renderer
{
public:
    Renderer(SDL_Renderer *r, TTF_Font *font, TTF_Font *sm);
    void drawAll(const Game &g, ParticleSystem &ps, StarField &sf,
                 ScorePopups &sp, FallingHazards &hz);
    void triggerQuote(const std::string &text);
    void mindBurst();

private:
    SDL_Renderer *ren;
    TTF_Font *font;
    TTF_Font *sm;
    std::vector<HeadQuote> headQuotes;
    int pulseTimer = 0;
    int scoreFlashTimer = 0;
    int displayedScore = 0;
    int mindIdleTimer = 0;
    int saiSpriteX = 0;
    int saiSpriteY = 0;
    std::vector<MindPop> mindPops;

    void drawBg();
    void drawGrid(const Game &g, ParticleSystem &ps);
    void drawGridStars();
    void drawGridHints();
    void drawActivePiece(const Piece &p);
    void drawGhost(const Game &g);
    void drawLeftPanel(const Game &g, ParticleSystem &ps);
    void drawRightPanel(const Game &g);
    void drawHeadQuotes();
    void drawGameOver(const Game &g);

    void uiPanel(int x, int y, int w, int h, SDL_Color border);
    void neonBorder(int x, int y, int w, int h, SDL_Color c);
    void solidBlock(int x, int y, SDL_Color c);
    void glowBlock(int x, int y, SDL_Color c);
    void txt(const std::string &s, int x, int y,
             SDL_Color c, TTF_Font *f = nullptr);
    void clearTxt(const std::string &s, int x, int y,
                  SDL_Color c, TTF_Font *f = nullptr);
    void statBox(int x, int y, int w, int h,
                 const char *label, const std::string &val,
                 SDL_Color valCol, bool flash);
};
