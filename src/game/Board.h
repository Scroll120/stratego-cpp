
#ifndef BOARD_H
#define BOARD_H

#include "../pieces/piece.h"
#include <vector>

struct Point {
    int x;
    int y;

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

class Board {
private:
    Piece* grid[10][10];
    std::vector<Point> obstacles;

public:
    Board();

    bool movePiece(int fromX, int fromY, int toX, int toY);
    Piece* getPiece(int x, int y);
    bool isMoveValid(int fromX, int fromY, int toX, int toY);

    void placePiece(int x, int y, Piece* piece);
    void addObstacle(int x, int y);
    bool isObstacle(int x, int y) const;
};

#endif //BOARD_H
