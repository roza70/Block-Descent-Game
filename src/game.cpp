#include "game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

static const Point SHAPES[PIECE_COUNT][4][4] = {
    {{{0, 1}, {1, 1}, {2, 1}, {3, 1}},
     {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
     {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
     {{1, 0}, {1, 1}, {1, 2}, {1, 3}}},
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
     {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
     {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
     {{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
    {{{1, 0}, {0, 1}, {1, 1}, {2, 1}},
     {{0, 0}, {0, 1}, {1, 1}, {0, 2}},
     {{0, 0}, {1, 0}, {2, 0}, {1, 1}},
     {{1, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
     {{1, 0}, {2, 0}, {0, 1}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 1}, {1, 2}}},
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
     {{1, 0}, {0, 1}, {1, 1}, {0, 2}},
     {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
     {{1, 0}, {0, 1}, {1, 1}, {0, 2}}},
    {{{2, 0}, {0, 1}, {1, 1}, {2, 1}},
     {{0, 0}, {0, 1}, {0, 2}, {1, 2}},
     {{0, 0}, {1, 0}, {2, 0}, {0, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {1, 2}}},
    {{{0, 0}, {0, 1}, {1, 1}, {2, 1}},
     {{0, 0}, {1, 0}, {0, 1}, {0, 2}},
     {{0, 0}, {1, 0}, {2, 0}, {2, 1}},
     {{1, 0}, {1, 1}, {0, 2}, {1, 2}}},
};

static const char *QUOTES[12] = {
    "Let's code!",
    "Running loops!",
    "Stack cleared!",
    "Debug mode ON",
    "O(1) speed!",
    "NULL pointer?",
    "Git commit!",
    "10 bil percent!",
    "New record!",
    "Segfault? Never!",
    "Science wins!",
    "Calculate this!",
};

Game::Game()
    : gameOver(false), activePiece(false), waitingSpawn(false),
      score(0), level(1), linesCleared(0), highScore(0),
      tickCounter(0), ticksPerDrop(28), flashTimer(0), hazardStun(0),
      newQuote(false)
{
    srand((unsigned)time(nullptr));
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            grid[r][c] = 0;
    loadHighScore();
    currentQuote = "Ready!";
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
    for (int i = 0; i < 4; i++)
        p.blocks[i] = SHAPES[t][0][i];
    return p;
}

int Game::spawnYFor(const Piece &p) const
{
    int minY = p.blocks[0].y;
    for (int i = 1; i < 4; i++)
        minY = std::min(minY, p.blocks[i].y);
    return -minY - 1;
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
    for (int i = 0; i < 4; i++)
    {
        int nx = p.x + p.blocks[i].x;
        int ny = p.y + p.blocks[i].y;
        if (nx < 0 || nx >= COLS)
            return false;
        if (ny < 0)
            continue;
        if (ny >= ROWS)
            return false;
        if (grid[ny][nx] != 0)
            return false;
    }
    return true;
}

void Game::spawnPiece()
{
    current = next;
    current.x = COLS / 2 - 2;
    current.y = spawnYFor(current);
    next = makePiece((PieceType)(rand() % PIECE_COUNT));

    if (!isValid(current))
    {
        current.y++;
        if (!isValid(current))
        {
            gameOver = true;
            activePiece = false;
            currentQuote = "Game over...";
            newQuote = true;
            saveHighScore();
            return;
        }
    }
    activePiece = true;
    tickCounter = 0;
}

void Game::lockPiece()
{
    for (int i = 0; i < 4; i++)
    {
        int nx = current.x + current.blocks[i].x;
        int ny = current.y + current.blocks[i].y;
        if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS)
            grid[ny][nx] = (int)current.type + 1;
    }

    activePiece = false;
    clearLines();

    if (flashTimer > 0)
        waitingSpawn = true;
    else
        spawnPiece();
}

void Game::clearLines()
{
    flashRows.clear();
    for (int r = 0; r < ROWS; r++)
    {
        bool full = true;
        for (int c = 0; c < COLS; c++)
        {
            if (grid[r][c] == 0)
            {
                full = false;
                break;
            }
        }
        if (full)
            flashRows.push_back(r);
    }

    if (flashRows.empty())
        return;

    flashTimer = 18;
    int n = (int)flashRows.size();
    int pts[] = {0, 100, 300, 500, 800};
    score += pts[std::min(n, 4)] * level;
    linesCleared += n;
    level = linesCleared / 10 + 1;
    ticksPerDrop = std::max(6, 42 - (level - 1) * 4);
    if (score > highScore)
        highScore = score;
    currentQuote = QUOTES[rand() % 12];
    newQuote = true;
}

void Game::applyPendingClears()
{
    if (flashRows.empty())
        return;

    for (int i = (int)flashRows.size() - 1; i >= 0; i--)
    {
        int row = flashRows[i];
        for (int rr = row; rr > 0; rr--)
            for (int c = 0; c < COLS; c++)
                grid[rr][c] = grid[rr - 1][c];
        for (int c = 0; c < COLS; c++)
            grid[0][c] = 0;
    }
    flashRows.clear();
}

void Game::applyHazardHit()
{
    hazardStun = 14;
    score = std::max(0, score - 15);
    currentQuote = "Star bump!";
    newQuote = true;
}

void Game::update()
{
    if (gameOver)
        return;

    newQuote = false;

    if (hazardStun > 0)
    {
        hazardStun--;
        return;
    }

    if (flashTimer > 0)
    {
        flashTimer--;
        if (flashTimer == 0)
        {
            applyPendingClears();
            if (waitingSpawn)
            {
                waitingSpawn = false;
                spawnPiece();
            }
        }
        return;
    }

    if (!activePiece)
        return;

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
    if (gameOver || !activePiece || isFlashing())
        return;
    Piece p = current;
    p.x--;
    if (isValid(p))
        current = p;
}

void Game::moveRight()
{
    if (gameOver || !activePiece || isFlashing())
        return;
    Piece p = current;
    p.x++;
    if (isValid(p))
        current = p;
}

void Game::moveDown()
{
    if (gameOver || !activePiece || isFlashing())
        return;
    Piece p = current;
    p.y++;
    if (isValid(p))
    {
        current = p;
        score++;
        tickCounter = 0;
    }
    else
        lockPiece();
}

void Game::rotate()
{
    if (gameOver || !activePiece || isFlashing())
        return;
    Piece p = rotatePiece(current);
    if (isValid(p))
    {
        current = p;
        return;
    }
    p.x++;
    if (isValid(p))
    {
        current = p;
        return;
    }
    p.x -= 2;
    if (isValid(p))
    {
        current = p;
        return;
    }
    p.x++;
    p.y--;
    if (isValid(p))
        current = p;
}

void Game::hardDrop()
{
    if (gameOver || !activePiece || isFlashing())
        return;
    int dropped = 0;
    while (true)
    {
        Piece p = current;
        p.y++;
        if (isValid(p))
        {
            current = p;
            dropped++;
        }
        else
        {
            lockPiece();
            break;
        }
    }
    score += dropped * 2;
    currentQuote = "Hard drop!";
    newQuote = true;
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
