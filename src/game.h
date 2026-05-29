#pragma once
#include <vector>
#include <string>

const int COLS = 10;
const int ROWS = 20;
const int BLOCK_SIZE = 30;

enum PieceType
{
    ALGORITHM,
    BINARY,
    FUNCTION,
    ARRAY,
    POINTER,
    LOOP,
    STACK,
    PIECE_COUNT
};

struct Point
{
    int x, y;
};

struct Piece
{
    PieceType type;
    Point blocks[4];
    int x, y, rotation;
};

class Game
{
public:
    Game();
    void update();
    void moveLeft();
    void moveRight();
    void moveDown();
    void rotate();
    void hardDrop();
    void applyHazardHit();

    bool isOver() const { return gameOver; }
    bool isStunned() const { return hazardStun > 0; }
    bool isFlashing() const { return flashTimer > 0; }
    bool hasActivePiece() const { return activePiece; }
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getHighScore() const { return highScore; }
    int getLinesCleared() const { return linesCleared; }
    const std::vector<int> &getFlashRows() const { return flashRows; }

    int grid[ROWS][COLS];
    Piece current, next;
    std::string currentQuote;
    bool newQuote;

private:
    bool gameOver;
    bool activePiece;
    bool waitingSpawn;
    int score, level, linesCleared, highScore;
    int tickCounter, ticksPerDrop, flashTimer, hazardStun;
    std::vector<int> flashRows;

    void spawnPiece();
    int spawnYFor(const Piece &p) const;
    bool isValid(const Piece &p) const;
    void lockPiece();
    void clearLines();
    void applyPendingClears();
    Piece makePiece(PieceType t) const;
    Piece rotatePiece(const Piece &p) const;
    void loadHighScore();
    void saveHighScore();
};
