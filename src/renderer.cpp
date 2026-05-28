#include "renderer.h"
#include "sprite.h"
#include <string>
#include <algorithm>

const int GX = 160;
const int GY = 40;
const int PX = 10;

Renderer::Renderer(SDL_Renderer *r, TTF_Font *f, TTF_Font *sf)
    : ren(r), font(f), smallFont(sf) {}

void Renderer::drawBlock(int x, int y, SDL_Color col)
{
    SDL_Rect shadow = {x + 2, y + 2, BLOCK_SIZE - 2, BLOCK_SIZE - 2};
    SDL_SetRenderDrawColor(ren,
                           col.r * 0.3, col.g * 0.3, col.b * 0.3, 255);
    SDL_RenderFillRect(ren, &shadow);

    SDL_Rect rect = {x, y, BLOCK_SIZE - 2, BLOCK_SIZE - 2};
    SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, 255);
    SDL_RenderFillRect(ren, &rect);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 80);
    SDL_RenderDrawLine(ren, x + 1, y + 1, x + BLOCK_SIZE - 4, y + 1);
    SDL_RenderDrawLine(ren, x + 1, y + 1, x + 1, y + 8);
}

void Renderer::drawText(const std::string &t, int x, int y,
                        SDL_Color col, TTF_Font *f)
{
    TTF_Font *use = f ? f : font;
    if (!use)
        return;
    SDL_Surface *s = TTF_RenderText_Blended(use, t.c_str(), col);
    if (!s)
        return;
    SDL_Texture *tx = SDL_CreateTextureFromSurface(ren, s);
    SDL_Rect dst = {x, y, s->w, s->h};
    SDL_RenderCopy(ren, tx, nullptr, &dst);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(tx);
}

void Renderer::drawBackground()
{
    SDL_SetRenderDrawColor(ren, 10, 12, 20, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 20, 25, 40, 255);
    for (int y = 0; y < 680; y += 20)
        for (int x = 0; x < 700; x += 20)
            SDL_RenderDrawPoint(ren, x, y);
}

void Renderer::drawGrid(const Game &g, ParticleSystem &ps)
{
    SDL_SetRenderDrawColor(ren, 40, 80, 120, 255);
    SDL_Rect border = {GX - 2, GY - 2,
                       COLS * BLOCK_SIZE + 4, ROWS * BLOCK_SIZE + 4};
    SDL_RenderDrawRect(ren, &border);

    SDL_SetRenderDrawColor(ren, 20, 30, 45, 255);
    for (int r = 0; r <= ROWS; r++)
        SDL_RenderDrawLine(ren, GX, GY + r * BLOCK_SIZE,
                           GX + COLS * BLOCK_SIZE, GY + r * BLOCK_SIZE);
    for (int c = 0; c <= COLS; c++)
        SDL_RenderDrawLine(ren, GX + c * BLOCK_SIZE, GY,
                           GX + c * BLOCK_SIZE, GY + ROWS * BLOCK_SIZE);

    bool flashing = g.isFlashing();
    const auto &frows = g.getFlashRows();

    for (int r = 0; r < ROWS; r++)
    {
        bool isFlashRow = flashing &&
                          std::find(frows.begin(), frows.end(), r) != frows.end();
        for (int c = 0; c < COLS; c++)
        {
            if (g.grid[r][c] != 0)
            {
                SDL_Color col = isFlashRow
                                    ? SDL_Color{255, 255, 255, 255}
                                    : PIECE_COLORS[g.grid[r][c] - 1];
                if (isFlashRow && rand() % 4 == 0)
                    ps.emit(GX + c * BLOCK_SIZE + 15,
                            GY + r * BLOCK_SIZE + 15,
                            PIECE_COLORS[g.grid[r][c] - 1], 3);
                drawBlock(GX + c * BLOCK_SIZE,
                          GY + r * BLOCK_SIZE, col);
            }
        }
    }
}

void Renderer::drawGhost(const Game &g)
{
    Piece ghost = g.current;
    while (true)
    {
        Piece b = ghost;
        b.y++;
        bool ok = true;
        for (auto &bl : b.blocks)
        {
            int nx = b.x + bl.x, ny = b.y + bl.y;
            if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS || g.grid[ny][nx])
            {
                ok = false;
                break;
            }
        }
        if (ok)
            ghost = b;
        else
            break;
    }
    SDL_Color col = PIECE_COLORS[ghost.type];
    for (auto &b : ghost.blocks)
    {
        SDL_Rect r = {GX + (ghost.x + b.x) * BLOCK_SIZE + 1,
                      GY + (ghost.y + b.y) * BLOCK_SIZE + 1,
                      BLOCK_SIZE - 2, BLOCK_SIZE - 2};
        SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, 50);
        SDL_RenderFillRect(ren, &r);
        SDL_SetRenderDrawColor(ren, col.r, col.g, col.b, 120);
        SDL_RenderDrawRect(ren, &r);
    }
}

void Renderer::drawPiece(const Piece &p, int ox, int oy)
{
    SDL_Color col = PIECE_COLORS[p.type];
    for (auto &b : p.blocks)
        drawBlock(ox + (p.x + b.x) * BLOCK_SIZE,
                  oy + (p.y + b.y) * BLOCK_SIZE, col);
}

void Renderer::drawSidePanel(const Game &g)
{
    SDL_Color white = {220, 220, 220, 255};
    SDL_Color cyan = {0, 255, 200, 255};
    SDL_Color yellow = {255, 220, 50, 255};
    SDL_Color dim = {100, 120, 140, 255};
    SDL_Color green = {100, 255, 150, 255};

    int rx = GX + COLS * BLOCK_SIZE + 18;

    // Left panel
    drawText("SAI NANAMI", PX, 15, cyan, smallFont);
    drawText("PROGRAMMER", PX, 32, dim, smallFont);
    drawSaiNanami(ren, PX, 55);

    // Quote bubble
    SDL_SetRenderDrawColor(ren, 20, 35, 55, 200);
    SDL_Rect bubble = {PX - 2, 162, 138, 36};
    SDL_RenderFillRect(ren, &bubble);
    SDL_SetRenderDrawColor(ren, 0, 255, 200, 80);
    SDL_RenderDrawRect(ren, &bubble);
    drawText(g.currentQuote, PX + 2, 170, cyan, smallFont);

    // Controls
    drawText("CONTROLS", PX, 215, yellow, smallFont);
    drawText("< >  Move", PX, 233, white, smallFont);
    drawText("^    Rotate", PX, 249, white, smallFont);
    drawText("v    Soft", PX, 265, white, smallFont);
    drawText("SPC  Drop", PX, 281, white, smallFont);
    drawText("R    Restart", PX, 297, white, smallFont);

    // Right panel
    drawText("SCORE", rx, 50, dim, smallFont);
    drawText(std::to_string(g.getScore()), rx, 66, yellow);
    drawText("BEST", rx, 110, dim, smallFont);
    drawText(std::to_string(g.getHighScore()), rx, 126, green);
    drawText("LEVEL", rx, 170, dim, smallFont);
    drawText(std::to_string(g.getLevel()), rx, 186, cyan);
    drawText("LINES", rx, 230, dim, smallFont);
    drawText(std::to_string(g.getLinesCleared()), rx, 246, white);

    // Next piece box
    drawText("NEXT", rx, 295, dim, smallFont);
    SDL_SetRenderDrawColor(ren, 15, 22, 38, 255);
    SDL_Rect nb = {rx, 315, 120, 90};
    SDL_RenderFillRect(ren, &nb);
    SDL_SetRenderDrawColor(ren, 40, 80, 120, 180);
    SDL_RenderDrawRect(ren, &nb);

    Piece preview = g.next;
    preview.x = 1;
    preview.y = 0;
    for (auto &b : preview.blocks)
        drawBlock(rx + 4 + (preview.x + b.x) * 28,
                  319 + (preview.y + b.y) * 28,
                  PIECE_COLORS[preview.type]);

    drawText(PIECE_NAMES[g.next.type], rx, 415,
             PIECE_COLORS[g.next.type], smallFont);

    // Game over
    if (g.isOver())
    {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 180);
        SDL_Rect ov = {GX, GY, COLS * BLOCK_SIZE, ROWS * BLOCK_SIZE};
        SDL_RenderFillRect(ren, &ov);
        SDL_Color red = {255, 80, 80, 255};
        drawText("GAME OVER", GX + 18, GY + 240, red);
        drawText("Press R", GX + 30, GY + 275, white, smallFont);
    }
}

void Renderer::drawAll(const Game &g, ParticleSystem &ps)
{
    drawBackground();
    drawGrid(g, ps);
    if (!g.isOver() && !g.isFlashing())
        drawGhost(g);
    if (!g.isOver())
        drawPiece(g.current, GX, GY);
    ps.update();
    ps.draw(ren);
    drawSidePanel(g);
    SDL_RenderPresent(ren);
}