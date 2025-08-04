#ifndef MINER_H
#define MINER_H

#include "Piece.h"

#include <stdlib.h>

class Miner : public Piece {
public:
  Miner() { rank = 3; }

  bool canMove() const override { return true; }

  int attack(Piece* other) override {
    if (other->getRank() == 11) {
      return 1;
    }
    return rank - other->getRank();
  }
  bool defuseBomb(Piece* otherPiece) {
    return otherPiece->getRank() == 11;
  }

  bool canMoveDirection(int x, int y) const override {
    return (abs(x) == 1 && y == 0) || (abs(y) == 1 && x == 0);
  }
};

#endif // MINER_H

