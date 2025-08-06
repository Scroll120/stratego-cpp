#ifndef BOMB_H
#define BOMB_H
#include "piece.h"

class Bomb : public Piece {
public:
  Bomb() { rank = 11;};
  bool canMove() const override{ return false; };
  int attack(Piece *other) override {return -999;};
  bool canMoveDirection(int deltaX, int deltaY) const override {
    return false;
  }
};
#endif //BOMB_H
