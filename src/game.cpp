#include "game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

static const Point SHAPES[PIECE_COUNT][4][4] = {
    {{{0, 1}, {1, 1}, {2, 1}, {3, 1}}, {{2, 0}, {2, 1}, {2, 2}, {2, 3}}, {{0, 2}, {1, 2}, {2, 2}, {3, 2}}, {{1, 0}, {1, 1}, {1, 2}, {1, 3}}},
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
    {{{1, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {0, 1}, {1, 1}, {0, 2}}, {{0, 0}, {1, 0}, {2, 0}, {1, 1}}, {{1, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}, {{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {{0, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {{1, 0}, {0, 1}, {1, 1}, {0, 2}}, {{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {{1, 0}, {0, 1}, {1, 1}, {0, 2}}},
    {{{2, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {0, 1}, {0, 2}, {1, 2}}, {{0, 0}, {1, 0}, {2, 0}, {0, 1}}, {{0, 0}, {1, 0}, {1, 1}, {1, 2}}},
    {{{0, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {1, 0}, {0, 1}, {0, 2}}, {{0, 0}, {1, 0}, {2, 0}, {2, 1}}, {{1, 0}, {1, 1}, {0, 2}, {1, 2}}},
};

static const char *QUOTES[] = {
    "Initializing...",
    "Running loops!",
    "Stack overflow!",
    "Debug mode ON",
    "O(1) clear!",
    "Null pointer?",
    "Compiling...",
    "Git commit!",
    "New high score!",
    "Segfault? No!",
};

Game::Game() : gameOver(false), score(0), level(1),
               linesCleared(0), tickCounter(0),
               ticksPerDrop(45), flashTimer(0)
{
    srand(time(nullptr));
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            grid[r][c] = 0;
    loadHighScore();
    currentQuote = "Sai Nanami ready!";
    next = makePiece((PieceType)(rand() % PIECE_COUNT));
    spawnPiece();
}

Piece Game::makePiece(PieceType t) const
{
    Piece p;
    p.type = t;
    p.rotation = 0;
    p.x = COLS / 2 - 2;
    p.y = 0;
    p.blocks.resize(4);
    for (int i = 0; i < 4; i++)
        p.blocks[i] = SHAPES[t][0][i];
    return p;
}

Piece Game::rotatePiece(const Piece &p) const
{
    Piece r = p;
    r.rotation = (p.rotation + 1) % 4;
    for (int i = 0; i < 4; i++)
        r.blocks[i] = SHAPES[p.type][r.rotation][i];
    return r;
}

bool Game::isValid(const Piece &p) const
{
    for (auto &b : p.blocks)
    {
        int nx = p.x + b.x, ny = p.y + b.y;
        if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS)
            return false;
        if (grid[ny][nx])
            return false;
    }
    return true;
}

void Game::spawnPiece()
{
    current = next;
    current.x = COLS / 2 - 2;
    current.y = 0;
    next = makePiece((PieceType)(rand() % PIECE_COUNT));
    if (!isValid(current))
    {
        gameOver = true;
        saveHighScore();
    }
}

void Game::lockPiece()
{
    for (auto &b : current.blocks)
        grid[current.y + b.y][current.x + b.x] = current.type + 1;
    clearLines();
    spawnPiece();
}

void Game::clearLines()
{
    flashRows.clear();
    for (int r = 0; r < ROWS; r++)
    {
        bool full = true;
        for (int c = 0; c < COLS; c++)
            if (!grid[r][c])
            {
                full = false;
                break;
            }
        if (full)
            flashRows.push_back(r);
    }
    if (!flashRows.empty())
    {
        flashTimer = 12;
        int n = (int)flashRows.size();
        int pts[] = {0, 100, 300, 500, 800};
        score += pts[std::min(n, 4)] * level;
        linesCleared += n;
        level = linesCleared / 10 + 1;
        ticksPerDrop = std::max(5, 45 - (level - 1) * 4);
        if (score > highScore)
            highScore = score;
        updateQuote();
        for (int i = (int)flashRows.size() - 1; i >= 0; i--)
        {
            int r = flashRows[i];
            for (int rr = r; rr > 0; rr--)
                for (int c = 0; c < COLS; c++)
                    grid[rr][c] = grid[rr - 1][c];
            for (int c = 0; c < COLS; c++)
                grid[0][c] = 0;
        }
    }
}

void Game::update()
{
    if (gameOver)
        return;
    if (flashTimer > 0)
    {
        flashTimer--;
        return;
    }
    tickCounter++;
    if (tickCounter >= ticksPerDrop)
    {
        tickCounter = 0;
        Piece moved = current;
        moved.y++;
        if (isValid(moved))
            current = moved;
        else
            lockPiece();
    }
}

void Game::moveLeft()
{
    Piece p = current;
    p.x--;
    if (isValid(p))
        current = p;
}
void Game::moveRight()
{
    Piece p = current;
    p.x++;
    if (isValid(p))
        current = p;
}
void Game::moveDown()
{
    Piece p = current;
    p.y++;
    if (isValid(p))
    {
        current = p;
        score++;
    }
    else
        lockPiece();
}
void Game::rotate()
{
    Piece p = rotatePiece(current);
    if (isValid(p))
        current = p;
}
void Game::hardDrop()
{
    while (true)
    {
        Piece p = current;
        p.y++;
        if (isValid(p))
        {
            current = p;
            score += 2;
        }
        else
        {
            lockPiece();
            break;
        }
    }
}
void Game::updateQuote()
{
    currentQuote = QUOTES[rand() % 10];
}
void Game::loadHighScore()
{
    std::ifstream f("highscore.dat");
    if (f.is_open())
        f >> highScore;
    else
        highScore = 0;
}
void Game::saveHighScore()
{
    std::ofstream f("highscore.dat");
    if (f.is_open())
        f << highScore;
}