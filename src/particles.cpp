#include "particles.h"
#include <cstdlib>
#include <cmath>

void ParticleSystem::emit(int x, int y, SDL_Color color, int count)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.x = (float)x;
        p.y = (float)y;
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 1.0f + (rand() % 30) / 10.0f;
        p.vx = cosf(angle) * speed;
        p.vy = sinf(angle) * speed;
        p.maxLife = p.life = 30 + rand() % 20;
        p.color = color;
        particles.push_back(p);
    }
}

void ParticleSystem::update()
{
    for (auto &p : particles)
    {
        p.x += p.vx;
        p.y += p.vy;
        p.vy += 0.1f;
        p.life--;
    }
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                       [](const Particle &p)
                       { return p.life <= 0; }),
        particles.end());
}

void ParticleSystem::draw(SDL_Renderer *ren)
{
    for (auto &p : particles)
    {
        float alpha = (float)p.life / p.maxLife;
        SDL_SetRenderDrawColor(ren,
                               p.color.r, p.color.g, p.color.b,
                               (Uint8)(alpha * 255));
        SDL_Rect r = {(int)p.x, (int)p.y, 3, 3};
        SDL_RenderFillRect(ren, &r);
    }
}