#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

struct Particle
{
    float x, y, vx, vy;
    int life, maxLife;
    SDL_Color color;
    int size;
    bool glow;
};

struct Star
{
    float x, y, speed;
    int brightness, size;
    float twinkle;
    SDL_Color tint;
};

struct FloatingScore
{
    std::string text;
    float x, y, scale;
    int timer, maxTimer;
    SDL_Color color;
};

struct FallingHazard
{
    float x, y, vy, rot, rotSpd;
    int kind, size;
};

class ParticleSystem
{
public:
    void emit(int x, int y, SDL_Color color, int count = 20);
    void emitSpark(int x, int y, SDL_Color color, int count = 3);
    void update();
    void draw(SDL_Renderer *ren);

private:
    std::vector<Particle> particles;
};

class StarField
{
public:
    StarField();
    void update();
    void draw(SDL_Renderer *ren);

private:
    std::vector<Star> stars;
};

class ScorePopups
{
public:
    void emit(int x, int y, const std::string &text, SDL_Color color);
    void update();
    void draw(SDL_Renderer *ren, TTF_Font *font);

private:
    std::vector<FloatingScore> scores;
};

class FallingHazards
{
public:
    void reset();
    void update(const Game &g, ParticleSystem &ps);
    void draw(SDL_Renderer *ren);
    bool checkPieceHit(const Game &g, ParticleSystem &ps);
    int getPerfectStreak() const;

private:
    std::vector<FallingHazard> hazards;
    int spawnTimer = 0;
    int perfectStreak = 0;

    void spawn(int gridX, int gridY, int gridW, int level);
    static void drawStarShape(SDL_Renderer *ren, int cx, int cy, int r,
                              float rot, SDL_Color c);
};
