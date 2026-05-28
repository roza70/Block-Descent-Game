#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

struct Particle
{
    float x, y, vx, vy;
    int life, maxLife;
    SDL_Color color;
};

class ParticleSystem
{
public:
    void emit(int x, int y, SDL_Color color, int count = 20);
    void update();
    void draw(SDL_Renderer *ren);

private:
    std::vector<Particle> particles;
};