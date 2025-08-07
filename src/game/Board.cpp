#include "Board.h"
#include <stdexcept>



Board::Board() {
    for (int i=0; i <10; i++) {
        for (int j= 0; j< 10; j++) {
            grid[i][j] = nullptr;
        }
    }
}

bool Board::isPathClear(int fromX, int fromY, int toX, int toY) {
    if (fromX != toX && fromY != toY) return false;

    int dx = (toX - fromX) == 0 ? 0 : (toX - fromX > 0 ? 1 : -1);
    int dy = (toY - fromY) == 0 ? 0 : (toY - fromY > 0 ? 1 : -1);

    int x = fromX + dx;
    int y = fromY + dy;

    while (x != toX || y != toY) {
        if (grid[x][y] != nullptr || isObstacle(x, y)) return false;
        x += dx;
        y += dy;
    }
    return true;
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
    if (x < 0 || x >= 10 || y < 0 || y >= 10 || isObstacle(x, y)) {
        throw std::out_of_range("x:y is out of range or obstacle in path");
    }
    grid[x][y] = piece;
}

Piece* Board::getPiece(int x, int y) {
    if (x < 0 || x >= 10 || y < 0 || y >= 10) return nullptr;
    return grid[x][y];
}

bool Board::isMoveValid(int fromX, int fromY, int toX, int toY) {
    if (toX < 0 || toX >= 10 || toY < 0 || toY >= 10) return false;
    if (isObstacle(toX, toY)) return false;

    Piece* piece = getPiece(fromX, fromY);
    if (!piece) return false;

    int dx = toX - fromX;
    int dy = toY - fromY;

    if (!piece->canMoveDirection(dx, dy)) return false;

    if (piece->getRank() == 2) {
        int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
        int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

        int currX = fromX + stepX;
        int currY = fromY + stepY;

        while (currX != toX || currY != toY) {
            if (getPiece(currX, currY) != nullptr || isObstacle(currX, currY)) {
                return false;
            }
            currX += stepX;
            currY += stepY;
        }
    } else {
        if (abs(dx) + abs(dy) != 1) return false;
    }

    Piece* targetPiece = getPiece(toX, toY);
    if (targetPiece && targetPiece->getOwnerId() == piece->getOwnerId()) {
        return false;
    }

    return true;
}

MoveResult Board::movePiece(int fromX, int fromY, int toX, int toY) {
    MoveResult result{false, nullptr, nullptr, toX, toY};

    if (!isMoveValid(fromX, fromY, toX, toY)) return result;

    Piece* attacker = grid[fromX][fromY];
    Piece* defender = grid[toX][toY];

    if (!attacker) return result;

    if (defender) {
        // Special case: defender is a Flag (rank 12)
        if (defender->getRank() == 12) {
            result.defenderDead = defender;
            grid[toX][toY] = attacker;
            grid[fromX][fromY] = nullptr;
            result.success = true;

            attacker->setIsRevealed(true);
            return result;
        }

        // Regular combat
        int attackResult = attacker->attack(defender);
        if (attackResult > 0) {
            result.defenderDead = defender;
            grid[toX][toY] = attacker;
            grid[fromX][fromY] = nullptr;
        } else if (attackResult == 0) {
            result.defenderDead = defender;
            result.attackerDead = attacker;
            grid[toX][toY] = nullptr;
            grid[fromX][fromY] = nullptr;
        } else {
            result.attackerDead = attacker;
            grid[fromX][fromY] = nullptr;
        }

        attacker->setIsRevealed(true);
        if (defender) defender->setIsRevealed(true);

        result.success = true;
    } else {
        // Move to empty tile
        grid[toX][toY] = attacker;
        grid[fromX][fromY] = nullptr;
        result.success = true;
    }

    return result;
}