#include "renderer.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>

static const int LX = 8;
static const int RX = GX + COLS * BLOCK_SIZE + 15;

static const SDL_Color UI_CYAN = {0, 230, 255, 255};
static const SDL_Color UI_LABEL = {130, 150, 175, 255};
static const SDL_Color UI_TEXT = {210, 220, 235, 255};
static const SDL_Color UI_PANEL = {8, 16, 32, 255};

Renderer::Renderer(SDL_Renderer *r, TTF_Font *f, TTF_Font *s)
    : ren(r), font(f), sm(s)
{
}

void Renderer::triggerQuote(const std::string &text)
{
    HeadQuote q;
    q.text = text;
    q.x = (float)saiHeadX(saiSpriteX) + (rand() % 14 - 7);
    q.y = (float)saiHeadY(saiSpriteY) - 14;
    q.vy = -0.65f;
    q.maxTimer = q.timer = 85 + (int)text.size() * 2;
    q.alpha = 255.0f;
    headQuotes.push_back(q);
    if ((int)headQuotes.size() > 3)
        headQuotes.erase(headQuotes.begin());
    mindBurst();
}

void Renderer::mindBurst()
{
    spawnMindBurst(mindPops, saiHeadX(saiSpriteX), saiHeadY(saiSpriteY) - 6, 2);
}

void Renderer::uiPanel(int x, int y, int w, int h, SDL_Color border)
{
    SDL_SetRenderDrawColor(ren, UI_PANEL.r, UI_PANEL.g, UI_PANEL.b, 245);
    SDL_Rect fill = {x, y, w, h};
    SDL_RenderFillRect(ren, &fill);
    SDL_SetRenderDrawColor(ren, border.r, border.g, border.b, 200);
    SDL_RenderDrawRect(ren, &fill);
}

void Renderer::neonBorder(int x, int y, int w, int h, SDL_Color c)
{
    for (int i = 2; i >= 1; i--)
    {
        SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, (Uint8)(18 * i));
        SDL_Rect r = {x - i, y - i, w + i * 2, h + i * 2};
        SDL_RenderDrawRect(ren, &r);
    }
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 255);
    SDL_Rect r = {x, y, w, h};
    SDL_RenderDrawRect(ren, &r);
}

void Renderer::solidBlock(int x, int y, SDL_Color c)
{
    SDL_SetRenderDrawColor(ren,
                           (Uint8)(c.r * 0.28), (Uint8)(c.g * 0.28), (Uint8)(c.b * 0.28), 255);
    SDL_Rect base = {x, y, BLOCK_SIZE - 1, BLOCK_SIZE - 1};
    SDL_RenderFillRect(ren, &base);
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 255);
    SDL_Rect face = {x + 1, y + 1, BLOCK_SIZE - 4, BLOCK_SIZE - 4};
    SDL_RenderFillRect(ren, &face);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 80);
    SDL_RenderDrawLine(ren, x + 2, y + 2, x + BLOCK_SIZE - 5, y + 2);
}

void Renderer::glowBlock(int x, int y, SDL_Color c)
{
    for (int i = 3; i >= 1; i--)
    {
        SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, (Uint8)(14 * i));
        SDL_Rect gr = {x - i, y - i,
                       BLOCK_SIZE - 1 + i * 2, BLOCK_SIZE - 1 + i * 2};
        SDL_RenderFillRect(ren, &gr);
    }
    solidBlock(x, y, c);
}

void Renderer::txt(const std::string &s, int x, int y,
                   SDL_Color c, TTF_Font *f)
{
    TTF_Font *use = f ? f : font;
    if (!use || s.empty())
        return;
    SDL_Surface *su = TTF_RenderText_Blended(use, s.c_str(), c);
    if (!su)
        return;
    SDL_Texture *tx = SDL_CreateTextureFromSurface(ren, su);
    SDL_Rect dst = {x, y, su->w, su->h};
    SDL_RenderCopy(ren, tx, nullptr, &dst);
    SDL_FreeSurface(su);
    SDL_DestroyTexture(tx);
}

void Renderer::clearTxt(const std::string &s, int x, int y,
                        SDL_Color c, TTF_Font *f)
{
    txt(s, x + 1, y + 1, {0, 0, 0, 140}, f);
    txt(s, x, y, c, f);
}

void Renderer::statBox(int x, int y, int w, int h,
                       const char *label, const std::string &val,
                       SDL_Color valCol, bool flash)
{
    SDL_Color border = {45, 75, 110, 255};
    if (flash)
        border = {100, 110, 60, 255};
    uiPanel(x, y, w, h, border);
    txt(label, x + 10, y + 7, UI_LABEL, sm);
    SDL_Color v = valCol;
    if (flash)
    {
        v.r = (Uint8)std::min(255, v.r + 30);
        v.g = (Uint8)std::min(255, v.g + 30);
    }
    txt(val, x + 10, y + 26, v);
}

void Renderer::drawBg()
{
    SDL_SetRenderDrawColor(ren, 4, 8, 18, 255);
    SDL_Rect full = {0, 0, 700, 680};
    SDL_RenderFillRect(ren, &full);
}

void Renderer::drawGridStars()
{
    int gw = COLS * BLOCK_SIZE;
    int gh = ROWS * BLOCK_SIZE;
    for (int i = 0; i < 40; i++)
    {
        int sx = GX + (i * 53 + pulseTimer / 3) % (gw - 6) + 3;
        int sy = GY + (i * 97 + pulseTimer / 5) % (gh - 6) + 3;
        Uint8 b = (Uint8)(90 + (i * 23) % 120);
        SDL_SetRenderDrawColor(ren, b, b, (Uint8)(b + 25), 200);
        int sz = (i % 4 == 0) ? 2 : 1;
        SDL_Rect p = {sx, sy, sz, sz};
        SDL_RenderFillRect(ren, &p);
    }
}

void Renderer::drawGrid(const Game &g, ParticleSystem &ps)
{
    SDL_Color border = {0, 200, 255, 255};
    neonBorder(GX - 2, GY - 2, COLS * BLOCK_SIZE + 4, ROWS * BLOCK_SIZE + 4, border);

    SDL_SetRenderDrawColor(ren, 6, 12, 28, 255);
    SDL_Rect gridBg = {GX, GY, COLS * BLOCK_SIZE, ROWS * BLOCK_SIZE};
    SDL_RenderFillRect(ren, &gridBg);

    drawGridStars();

    SDL_SetRenderDrawColor(ren, 18, 32, 52, 180);
    for (int r = 0; r <= ROWS; r++)
        SDL_RenderDrawLine(ren, GX, GY + r * BLOCK_SIZE,
                           GX + COLS * BLOCK_SIZE, GY + r * BLOCK_SIZE);
    for (int c = 0; c <= COLS; c++)
        SDL_RenderDrawLine(ren, GX + c * BLOCK_SIZE, GY,
                           GX + c * BLOCK_SIZE, GY + ROWS * BLOCK_SIZE);

    bool flash = g.isFlashing();
    const auto &fr = g.getFlashRows();
    for (int r = 0; r < ROWS; r++)
    {
        bool isF = flash &&
                   std::find(fr.begin(), fr.end(), r) != fr.end();
        for (int c = 0; c < COLS; c++)
        {
            if (!g.grid[r][c])
                continue;
            SDL_Color col = isF
                                ? SDL_Color{255, 255, 255, 255}
                                : PIECE_COLORS[g.grid[r][c] - 1];
            if (isF && rand() % 2 == 0)
                ps.emit(GX + c * BLOCK_SIZE + 15,
                        GY + r * BLOCK_SIZE + 15,
                        PIECE_COLORS[g.grid[r][c] - 1], 5);
            glowBlock(GX + c * BLOCK_SIZE, GY + r * BLOCK_SIZE, col);
        }
    }
}

void Renderer::drawGridHints()
{
    int y = GY + ROWS * BLOCK_SIZE + 10;
    SDL_Color dim = {70, 95, 125, 255};
    txt("< >", GX + 12, y, dim, sm);
    txt("^", GX + 52, y, dim, sm);
    txt("v", GX + 72, y, dim, sm);
    txt("SPC", GX + 130, y, dim, sm);
    txt("R", GX + 200, y, dim, sm);
}

void Renderer::drawGhost(const Game &g)
{
    Piece gh = g.current;
    while (true)
    {
        Piece b = gh;
        b.y++;
        bool ok = true;
        for (int i = 0; i < 4; i++)
        {
            int nx = b.x + b.blocks[i].x;
            int ny = b.y + b.blocks[i].y;
            if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS || g.grid[ny][nx])
            {
                ok = false;
                break;
            }
        }
        if (ok)
            gh = b;
        else
            break;
    }
    SDL_Color c = PIECE_COLORS[gh.type];
    for (int i = 0; i < 4; i++)
    {
        int py = gh.y + gh.blocks[i].y;
        if (py < 0)
            continue;
        SDL_Rect r = {
            GX + (gh.x + gh.blocks[i].x) * BLOCK_SIZE + 2,
            GY + py * BLOCK_SIZE + 2,
            BLOCK_SIZE - 4, BLOCK_SIZE - 4};
        SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 45);
        SDL_RenderFillRect(ren, &r);
        SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, 100);
        SDL_RenderDrawRect(ren, &r);
    }
}

void Renderer::drawActivePiece(const Piece &p)
{
    SDL_Color c = PIECE_COLORS[p.type];
    for (int i = 0; i < 4; i++)
    {
        int py = p.y + p.blocks[i].y;
        if (py < 0)
            continue;
        glowBlock(GX + (p.x + p.blocks[i].x) * BLOCK_SIZE,
                  GY + py * BLOCK_SIZE, c);
    }
}

void Renderer::drawHeadQuotes()
{
    for (auto it = headQuotes.begin(); it != headQuotes.end();)
    {
        it->timer--;
        it->y += it->vy;
        if (it->timer < 28)
            it->alpha = (it->timer / 28.0f) * 255.0f;
        if (it->timer <= 0)
        {
            it = headQuotes.erase(it);
            continue;
        }

        Uint8 a = (Uint8)std::max(0.0f, it->alpha);
        int hx = saiHeadX(saiSpriteX);
        int hy = saiHeadY(saiSpriteY);
        int tw = (int)it->text.size() * 7 + 14;
        int th = 20;
        int bx = (int)it->x - tw / 2;
        int by = (int)it->y - th;

        SDL_SetRenderDrawColor(ren, 10, 18, 35, (Uint8)(a * 0.9f));
        SDL_Rect bg = {bx, by, tw, th};
        SDL_RenderFillRect(ren, &bg);
        SDL_SetRenderDrawColor(ren, 0, 200, 230, a);
        SDL_RenderDrawRect(ren, &bg);
        SDL_RenderDrawLine(ren, hx, hy - 2, bx + tw / 2, by + th);

        clearTxt(it->text, bx + 5, by + 3, {220, 240, 255, a}, sm);
        ++it;
    }
}

void Renderer::drawLeftPanel(const Game &g, ParticleSystem &ps)
{
    pulseTimer++;
    float bob = sinf(pulseTimer * 0.07f) * 1.5f;

    saiSpriteX = LX + 38;
    saiSpriteY = 52;
    int portraitH = SAI_H * SAI_SCALE + 8;

    uiPanel(LX, 6, 148, 36, {40, 70, 110, 255});
    clearTxt("SAI NANAMI", LX + 10, 10, UI_CYAN, sm);
    clearTxt("PROGRAMMER", LX + 10, 24, UI_TEXT, sm);

    uiPanel(LX, 46, 148, portraitH + 8, {40, 70, 110, 255});
    drawSaiNanami(ren, saiSpriteX, saiSpriteY, 1.0f, bob);

    mindIdleTimer++;
    if (mindIdleTimer > 110)
    {
        mindIdleTimer = 0;
        MindPop m;
        m.icon = rand() % 6;
        m.x = (float)saiHeadX(saiSpriteX);
        m.y = (float)saiHeadY(saiSpriteY) - 10;
        m.vx = (rand() % 8 - 4) / 30.0f;
        m.vy = -0.45f;
        m.scale = 0.1f;
        m.rot = 0;
        m.maxLife = m.life = 22;
        mindPops.push_back(m);
    }
    updateMindPops(mindPops);
    drawMindPops(ren, mindPops);
    drawHeadQuotes();

    int cy = 54 + portraitH + 10;
    uiPanel(LX, cy, 148, 118, {40, 70, 110, 255});
    clearTxt("CONTROLS", LX + 10, cy + 6, UI_CYAN, sm);

    int ly = cy + 24;
    clearTxt("< >", LX + 10, ly, UI_CYAN, sm);
    txt("Move", LX + 44, ly, UI_TEXT, sm);
    ly += 16;
    clearTxt("^", LX + 10, ly, UI_CYAN, sm);
    txt("Rotate", LX + 44, ly, UI_TEXT, sm);
    ly += 16;
    clearTxt("v", LX + 10, ly, UI_CYAN, sm);
    txt("Soft drop", LX + 44, ly, UI_TEXT, sm);
    ly += 16;
    clearTxt("SPC", LX + 10, ly, UI_CYAN, sm);
    txt("Hard drop", LX + 44, ly, UI_TEXT, sm);
    ly += 16;
    clearTxt("R", LX + 10, ly, UI_CYAN, sm);
    txt("Restart", LX + 44, ly, UI_TEXT, sm);
    ly += 16;
    txt("Q  Quit", LX + 10, ly, UI_LABEL, sm);

    if (g.isOver())
        clearTxt("Press R!", LX + 40, ly + 14, {255, 130, 130, 255}, sm);
    (void)ps;
}

void Renderer::drawRightPanel(const Game &g)
{
    SDL_Color yellow = {255, 220, 80, 255};
    SDL_Color green = {100, 255, 150, 255};
    SDL_Color cyan = {100, 220, 255, 255};
    SDL_Color purple = {200, 140, 255, 255};

    int sc = g.getScore();
    if (sc != displayedScore)
    {
        if (sc > displayedScore)
            scoreFlashTimer = 15;
        displayedScore += std::max(1, (sc - displayedScore) / 6);
        if (displayedScore > sc)
            displayedScore = sc;
    }
    bool flash = scoreFlashTimer > 0;
    if (scoreFlashTimer > 0)
        scoreFlashTimer--;

    statBox(RX, 42, 130, 52, "SCORE", std::to_string(displayedScore), yellow, flash);
    statBox(RX, 104, 130, 52, "BEST", std::to_string(g.getHighScore()), green, false);
    statBox(RX, 166, 130, 52, "LEVEL", std::to_string(g.getLevel()), cyan, false);
    statBox(RX, 228, 130, 52, "LINES", std::to_string(g.getLinesCleared()), purple, false);

    txt("NEXT", RX + 10, 292, UI_LABEL, sm);
    SDL_Color nc = PIECE_COLORS[g.next.type];
    neonBorder(RX, 308, 130, 108, nc);

    SDL_SetRenderDrawColor(ren, 6, 12, 26, 255);
    SDL_Rect nb = {RX + 1, 309, 128, 106};
    SDL_RenderFillRect(ren, &nb);

    Piece pv = g.next;
    pv.x = 1;
    pv.y = 0;
    for (auto &b : pv.blocks)
        glowBlock(RX + 10 + (pv.x + b.x) * 26,
                  316 + (pv.y + b.y) * 26,
                  PIECE_COLORS[pv.type]);

    txt(PIECE_NAMES[g.next.type], RX + 8, 418, nc, sm);
}

void Renderer::drawGameOver(const Game &g)
{
    if (!g.isOver())
        return;
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 180);
    SDL_Rect ov = {GX, GY, COLS * BLOCK_SIZE, ROWS * BLOCK_SIZE};
    SDL_RenderFillRect(ren, &ov);
    clearTxt("GAME OVER", GX + 50, GY + 218, {255, 100, 100, 255});
    txt("Press R to restart", GX + 28, GY + 256, UI_TEXT, sm);
}

void Renderer::drawAll(const Game &g,
                       ParticleSystem &ps, StarField &sf, ScorePopups &sp,
                       FallingHazards &hz)
{
    drawBg();
    sf.update();
    sf.draw(ren);

    drawGrid(g, ps);
    hz.draw(ren);

    if (g.hasActivePiece() && !g.isFlashing())
        drawGhost(g);
    if (g.hasActivePiece())
        drawActivePiece(g.current);

    drawGridHints();

    ps.update();
    ps.draw(ren);
    sp.update();
    sp.draw(ren, font);

    drawLeftPanel(g, ps);
    drawRightPanel(g);
    drawGameOver(g);
    SDL_RenderPresent(ren);
}
