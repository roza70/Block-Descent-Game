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
    std::vector<Point> blocks;
    int x, y;
    int rotation;
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

    bool isOver() const { return gameOver; }
    bool isFlashing() const { return flashTimer > 0; }
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getHighScore() const { return highScore; }
    int getLinesCleared() const { return linesCleared; }
    const std::vector<int> &
    getFlashRows() const { return flashRows; }

    int grid[ROWS][COLS];
    Piece current;
    Piece next;
    std::string currentQuote;

private:
    bool gameOver;
    int score, level, linesCleared, highScore;
    int tickCounter, ticksPerDrop;
    int flashTimer;
    std::vector<int> flashRows;

    void spawnPiece();
    bool isValid(const Piece &p) const;
    void lockPiece();
    void clearLines();
    Piece makePiece(PieceType t) const;
    Piece rotatePiece(const Piece &p) const;
    void loadHighScore();
    void saveHighScore();
    void updateQuote();
};