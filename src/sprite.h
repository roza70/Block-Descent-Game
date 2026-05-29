#pragma once
#include <SDL2/SDL.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>

// Chibi bust portrait — cute Sai
static const int SAI_W = 20;
static const int SAI_H = 22;
static const int SAI_SCALE = 4;

static const SDL_Color SAI_PAL[] = {
    {0, 0, 0, 0},
    {255, 210, 175, 255}, // 1 skin
    {88, 42, 18, 255},    // 2 hair dark
    {130, 62, 26, 255},   // 3 hair
    {178, 92, 38, 255},   // 4 hair highlight
    {252, 252, 255, 255}, // 5 shirt
    {218, 218, 228, 255}, // 6 shirt shade
    {255, 175, 195, 255}, // 7 blush
    {255, 255, 255, 255}, // 8 eye shine
    {72, 48, 32, 255},    // 9 eye
    {24, 20, 28, 255},    // 10 pupil
    {110, 175, 88, 255},  // 11 belt
    {200, 205, 215, 255}, // 12 buckle
    {175, 135, 100, 255}, // 13 neck
};

static const int SAI_PIX[SAI_H][SAI_W] = {
    {0, 0, 0, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 2, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 2, 0, 0, 0, 0},
    {0, 2, 3, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 3, 2, 0, 0, 0},
    {2, 3, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 3, 2, 0, 0},
    {2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 0, 0, 0},
    {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0},
    {0, 1, 1, 9, 10, 8, 1, 1, 1, 1, 8, 10, 9, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 9, 10, 10, 9, 1, 1, 1, 1, 9, 10, 10, 9, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 7, 1, 1, 1, 1, 1, 1, 1, 1, 7, 1, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 7, 1, 1, 13, 13, 1, 1, 7, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 13, 13, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 5, 5, 13, 13, 13, 13, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 5, 5, 6, 5, 5, 5, 5, 6, 5, 5, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 5, 5, 5, 6, 5, 6, 6, 5, 6, 5, 5, 5, 0, 0, 0, 0, 0, 0},
    {0, 5, 5, 5, 6, 5, 6, 5, 5, 6, 5, 6, 5, 5, 5, 0, 0, 0, 0, 0},
    {0, 5, 5, 5, 5, 5, 6, 5, 5, 6, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0},
    {0, 0, 11, 11, 11, 11, 12, 12, 12, 12, 11, 11, 11, 11, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

struct MindPop
{
    int icon;
    float x, y, vx, vy, scale, rot;
    int life, maxLife;
};

inline int saiHeadX(int px) { return px + SAI_W * SAI_SCALE / 2; }
inline int saiHeadY(int py) { return py + 6 * SAI_SCALE; }

inline void drawSaiNanami(SDL_Renderer *ren, int px, int py, float pulse, float bob)
{
    int drawY = py + (int)bob;
    (void)pulse;
    for (int row = 0; row < SAI_H; row++)
        for (int col = 0; col < SAI_W; col++)
        {
            int idx = SAI_PIX[row][col];
            if (!idx)
                continue;
            SDL_Color c = SAI_PAL[idx];
            SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 255);
            SDL_Rect r = {px + col * SAI_SCALE, drawY + row * SAI_SCALE,
                          SAI_SCALE, SAI_SCALE};
            SDL_RenderFillRect(ren, &r);
        }
}

static const int MIND_ICON_W = 6;
static const int MIND_PATTERNS[6][36] = {
    {0, 2, 2, 2, 0, 0, 2, 3, 3, 2, 0, 0, 2, 3, 3, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 2, 0, 0, 0, 2, 3, 2, 2, 0, 0, 2, 3, 3, 2, 0, 0, 2, 3, 2, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 2, 2, 0, 0, 2, 3, 3, 2, 0, 0, 2, 3, 3, 2, 0, 0, 2, 3, 3, 2, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 0},
    {0, 2, 2, 0, 0, 0, 2, 3, 3, 2, 0, 0, 2, 3, 3, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static const SDL_Color MIND_COLORS[6] = {
    {255, 230, 80, 255}, {0, 255, 220, 255}, {120, 255, 120, 255},
    {210, 80, 255, 255}, {255, 180, 50, 255}, {255, 130, 180, 255},
};

inline void drawMindIcon(SDL_Renderer *ren, int kind, int cx, int cy,
                         float scale, float rot, Uint8 alpha)
{
    if (kind < 0 || kind > 5)
        return;
    const int *pat = MIND_PATTERNS[kind];
    SDL_Color base = MIND_COLORS[kind];
    int sz = std::max(2, (int)(MIND_ICON_W * scale));
    float cs = cosf(rot), sn = sinf(rot);
    for (int row = 0; row < MIND_ICON_W; row++)
        for (int col = 0; col < MIND_ICON_W; col++)
        {
            int v = pat[row * MIND_ICON_W + col];
            if (!v)
                continue;
            SDL_Color c = (v == 3) ? base : SDL_Color{
                                                    (Uint8)(base.r * 0.4f),
                                                    (Uint8)(base.g * 0.4f),
                                                    (Uint8)(base.b * 0.4f), alpha};
            float fx = (col - MIND_ICON_W / 2.0f) * scale;
            float fy = (row - MIND_ICON_W / 2.0f) * scale;
            int px = cx + (int)(fx * cs - fy * sn);
            int py = cy + (int)(fx * sn + fy * cs);
            SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, alpha);
            SDL_Rect r = {px, py, sz, sz};
            SDL_RenderFillRect(ren, &r);
        }
}

inline void updateMindPops(std::vector<MindPop> &pops)
{
    for (auto &m : pops)
    {
        m.x += m.vx;
        m.y += m.vy;
        m.vy -= 0.015f;
        m.rot += 0.05f;
        m.life--;
        float t = 1.0f - (float)m.life / m.maxLife;
        if (t < 0.2f)
            m.scale = t / 0.2f * 1.2f;
        else if (t > 0.7f)
            m.scale = (1.0f - (t - 0.7f) / 0.3f) * 1.2f;
        else
            m.scale = 1.2f;
    }
    pops.erase(std::remove_if(pops.begin(), pops.end(),
                              [](const MindPop &m)
                              { return m.life <= 0; }),
               pops.end());
}

inline void drawMindPops(SDL_Renderer *ren, const std::vector<MindPop> &pops)
{
    for (const auto &m : pops)
    {
        float a = (float)m.life / m.maxLife;
        drawMindIcon(ren, m.icon, (int)m.x, (int)m.y, m.scale, m.rot,
                     (Uint8)(a * 255));
    }
}

inline void spawnMindBurst(std::vector<MindPop> &pops, int headX, int headY, int count = 3)
{
    for (int i = 0; i < count; i++)
    {
        MindPop m;
        m.icon = rand() % 6;
        m.x = (float)headX + (rand() % 30 - 15);
        m.y = (float)headY + (rand() % 10 - 5);
        m.vx = (rand() % 16 - 8) / 20.0f;
        m.vy = -0.7f - (rand() % 12) / 18.0f;
        m.scale = 0.1f;
        m.rot = (rand() % 628) / 100.0f;
        m.maxLife = m.life = 32 + rand() % 20;
        pops.push_back(m);
    }
}
