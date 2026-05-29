#include "particles.h"
#include <cstdlib>
#include <cmath>

static const int HGX = 158;
static const int HGY = 40;

void ParticleSystem::emit(int x, int y, SDL_Color color, int count)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.x = (float)x;
        p.y = (float)y;
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 1.5f + (rand() % 50) / 10.0f;
        p.vx = cosf(angle) * speed;
        p.vy = sinf(angle) * speed;
        p.maxLife = p.life = 30 + rand() % 30;
        p.color = color;
        p.size = 2 + rand() % 4;
        p.glow = true;
        particles.push_back(p);
    }
}

void ParticleSystem::emitSpark(int x, int y, SDL_Color color, int count)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.x = (float)x + (rand() % 10 - 5);
        p.y = (float)y + (rand() % 10 - 5);
        p.vx = (rand() % 20 - 10) / 12.0f;
        p.vy = -0.5f - (rand() % 20) / 15.0f;
        p.maxLife = p.life = 18 + rand() % 15;
        p.color = color;
        p.size = 1 + rand() % 2;
        p.glow = true;
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
        p.vx *= 0.96f;
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
        float a = (float)p.life / p.maxLife;
        Uint8 alpha = (Uint8)(a * 255);
        if (p.glow)
        {
            SDL_SetRenderDrawColor(ren, p.color.r, p.color.g, p.color.b,
                                   (Uint8)(alpha / 5));
            SDL_Rect gr = {(int)p.x - 2, (int)p.y - 2, p.size + 4, p.size + 4};
            SDL_RenderFillRect(ren, &gr);
        }
        SDL_SetRenderDrawColor(ren, p.color.r, p.color.g, p.color.b, alpha);
        SDL_Rect r = {(int)p.x, (int)p.y, p.size, p.size};
        SDL_RenderFillRect(ren, &r);
    }
}

StarField::StarField()
{
    SDL_Color tints[] = {
        {140, 140, 170, 255},
        {90, 120, 160, 255},
        {160, 120, 150, 255},
    };
    for (int i = 0; i < 60; i++)
    {
        Star s;
        s.x = (float)(rand() % 700);
        s.y = (float)(rand() % 680);
        s.speed = 0.1f + (rand() % 12) / 10.0f;
        s.brightness = 40 + rand() % 100;
        s.size = 1;
        s.twinkle = (float)(rand() % 628) / 100.0f;
        s.tint = tints[rand() % 3];
        stars.push_back(s);
    }
}

void StarField::update()
{
    for (auto &s : stars)
    {
        s.y += s.speed;
        s.twinkle += 0.05f;
        if (s.y > 680)
        {
            s.y = 0;
            s.x = (float)(rand() % 700);
        }
    }
}

void StarField::draw(SDL_Renderer *ren)
{
    for (auto &s : stars)
    {
        float flicker = 0.6f + 0.4f * sinf(s.twinkle);
        Uint8 b = (Uint8)(s.brightness * flicker);
        SDL_SetRenderDrawColor(ren,
                               (Uint8)(s.tint.r * b / 255),
                               (Uint8)(s.tint.g * b / 255),
                               (Uint8)(s.tint.b * b / 255), 255);
        SDL_Rect r = {(int)s.x, (int)s.y, s.size, s.size};
        SDL_RenderFillRect(ren, &r);
    }
}

void ScorePopups::emit(int x, int y, const std::string &text, SDL_Color color)
{
    FloatingScore s;
    s.text = text;
    s.x = (float)x;
    s.y = (float)y;
    s.scale = 0.5f;
    s.maxTimer = s.timer = 55;
    s.color = color;
    scores.push_back(s);
}

void ScorePopups::update()
{
    for (auto &s : scores)
    {
        s.y -= 1.4f;
        s.timer--;
        float t = 1.0f - (float)s.timer / s.maxTimer;
        if (t < 0.15f)
            s.scale = 0.5f + (t / 0.15f) * 0.8f;
        else if (t > 0.75f)
            s.scale = 1.3f * (1.0f - (t - 0.75f) / 0.25f);
        else
            s.scale = 1.3f;
    }
    scores.erase(
        std::remove_if(scores.begin(), scores.end(),
                       [](const FloatingScore &s)
                       { return s.timer <= 0; }),
        scores.end());
}

void ScorePopups::draw(SDL_Renderer *ren, TTF_Font *font)
{
    if (!font)
        return;
    for (auto &s : scores)
    {
        float a = (float)s.timer / s.maxTimer;
        SDL_Color c = {s.color.r, s.color.g, s.color.b, (Uint8)(a * 255)};
        SDL_Surface *su = TTF_RenderText_Blended(font, s.text.c_str(), c);
        if (!su)
            continue;
        int w = (int)(su->w * s.scale);
        int h = (int)(su->h * s.scale);
        SDL_Texture *tx = SDL_CreateTextureFromSurface(ren, su);
        SDL_Rect dst = {(int)s.x - w / 2, (int)s.y, w, h};
        SDL_RenderCopy(ren, tx, nullptr, &dst);
        SDL_FreeSurface(su);
        SDL_DestroyTexture(tx);
    }
}

void FallingHazards::reset()
{
    hazards.clear();
    spawnTimer = 40;
    perfectStreak = 0;
}

void FallingHazards::spawn(int gridX, int gridY, int gridW, int level)
{
    FallingHazard h;
    h.x = (float)(gridX + 20 + rand() % (gridW - 40));
    h.y = (float)gridY + 4;
    h.vy = 1.2f + level * 0.15f + (rand() % 10) / 10.0f;
    h.rot = (rand() % 628) / 100.0f;
    h.rotSpd = (rand() % 10 - 5) / 40.0f;
    h.kind = rand() % 2;
    h.size = 7 + rand() % 6;
    hazards.push_back(h);
}

void FallingHazards::drawStarShape(SDL_Renderer *ren, int cx, int cy, int r,
                                   float rot, SDL_Color c)
{
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
    for (int i = 0; i < 4; i++)
    {
        float a1 = rot + i * 1.5708f;
        float a2 = rot + i * 1.5708f + 0.35f;
        int x1 = cx + (int)(cosf(a1) * r);
        int y1 = cy + (int)(sinf(a1) * r);
        int x2 = cx + (int)(cosf(a2) * (r / 2));
        int y2 = cy + (int)(sinf(a2) * (r / 2));
        SDL_RenderDrawLine(ren, cx, cy, x1, y1);
        SDL_RenderDrawLine(ren, cx, cy, x2, y2);
    }
    SDL_Rect core = {cx - 2, cy - 2, 4, 4};
    SDL_RenderFillRect(ren, &core);
}

void FallingHazards::update(const Game &g, ParticleSystem &ps)
{
    if (g.isOver() || !g.hasActivePiece() || g.isFlashing())
        return;

    spawnTimer--;
    int interval = std::max(35, 110 - g.getLevel() * 8);
    if (spawnTimer <= 0)
    {
        spawn(HGX, HGY, COLS * BLOCK_SIZE, g.getLevel());
        spawnTimer = interval + rand() % 30;
    }

    int gridBot = HGY + ROWS * BLOCK_SIZE;
    for (auto &h : hazards)
    {
        h.y += h.vy;
        h.rot += h.rotSpd;

        int col = (int)((h.x - HGX) / BLOCK_SIZE);
        int row = (int)((h.y - HGY) / BLOCK_SIZE);
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS && g.grid[row][col])
        {
            ps.emit((int)h.x, (int)h.y, {255, 200, 80, 255}, 8);
            h.y = (float)gridBot + 50;
        }
    }

    hazards.erase(
        std::remove_if(hazards.begin(), hazards.end(),
                       [gridBot](const FallingHazard &h)
                       { return h.y > gridBot + 20; }),
        hazards.end());
}

void FallingHazards::draw(SDL_Renderer *ren)
{
    for (const auto &h : hazards)
    {
        SDL_Color c = h.kind == 0
                          ? SDL_Color{255, 220, 90, 220}
                          : SDL_Color{255, 140, 200, 200};
        drawStarShape(ren, (int)h.x, (int)h.y, h.size, h.rot, c);
    }
}

bool FallingHazards::checkPieceHit(const Game &g, ParticleSystem &ps)
{
    if (!g.hasActivePiece() || g.isOver())
        return false;

    const Piece &p = g.current;
    bool hit = false;

    for (auto it = hazards.begin(); it != hazards.end();)
    {
        bool got = false;
        for (int i = 0; i < 4; i++)
        {
            int bx = HGX + (p.x + p.blocks[i].x) * BLOCK_SIZE;
            int by = HGY + (p.y + p.blocks[i].y) * BLOCK_SIZE;
            if (p.y + p.blocks[i].y < 0)
                continue;
            float dx = it->x - (bx + BLOCK_SIZE / 2.0f);
            float dy = it->y - (by + BLOCK_SIZE / 2.0f);
            if (dx * dx + dy * dy < (BLOCK_SIZE * 0.55f) * (BLOCK_SIZE * 0.55f))
            {
                got = true;
                break;
            }
        }
        if (got)
        {
            ps.emit((int)it->x, (int)it->y, {255, 180, 60, 255}, 12);
            it = hazards.erase(it);
            hit = true;
            perfectStreak = 0;
        }
        else
            ++it;
    }

    if (!hit && g.hasActivePiece())
        perfectStreak++;

    return hit;
}

int FallingHazards::getPerfectStreak() const
{
    return perfectStreak;
}
