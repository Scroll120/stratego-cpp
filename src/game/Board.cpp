#include "Board.h"
#include <stdexcept>

Board::Board() {
    for (int i=0; i <10; i++) {
        for (int j= 0; j< 10; j++) {
            grid[i][j] = nullptr;
        }
    }
}

bool Board::isObstacle(int x, int y) const {
    for (const Point& obs : obstacles) {
        if (obs.x == x && obs.y == y) return true;
    }
    return false;
}

std::vector<Point> Board::getObstacles() {
    return obstacles;
}

void Board::addObstacle(int x, int y) {
    obstacles.push_back({x,y});
}

void Board::placePiece(int x, int y, Piece* piece) {
    if (x < 0 || x >= 10 || y < 0 || y > 10 || isObstacle(x, y)) {
        throw std::out_of_range("x:y is out of range or obstacle in path");
    }
    grid[x][y] = piece;
}

Piece* Board::getPiece(int x, int y) {
    if (x < 0 || x >= 10 || y < 0 || y > 10) return nullptr;
    return grid[x][y];
}

bool Board::isMoveValid(int fromX, int fromY, int toX, int toY) {
    if (toX < 0 || toX >= 10 || toY < 0 || toY >= 10) return false;
    if (isObstacle(toX, toY)) return false;

    Piece* piece = getPiece(fromX, fromY);
    if (!piece) return false;

    return piece->canMove();
}

bool Board::movePiece(int fromX, int fromY, int toX, int toY) {
    if (!isMoveValid(fromX, fromY, toX, toY)) return false;

    Piece* movingPiece = grid[fromX][fromY];
    Piece* targetPiece = grid[toX][toY];

    // down to change
    if (targetPiece) {
        int result = movingPiece->attack(targetPiece);
        if (result == 1) {
            delete targetPiece;
            grid[toX][toY] = movingPiece;
        } else if (result == 0) {
            delete targetPiece;
            delete movingPiece;
            grid[toX][toY] = nullptr;
        } else {
            delete movingPiece;
        }
        grid[fromX][fromY] = nullptr;
    } else {
        grid[toX][toY] = movingPiece;
        grid[fromX][fromY] = nullptr;
    }

    return true;
}