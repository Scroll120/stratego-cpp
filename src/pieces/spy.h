#ifndef SPY_H
#define SPY_H

#include "Piece.h"

#include <stdlib.h>

class Spy : public Piece {
public:
  Spy() { rank = 1; }

  bool canMove() const override { return true; }
  int attack(Piece* other) override {
    if (other->getRank() == 10) {
      return 1;
    }
    return rank - other->getRank();
  }

  bool canMoveDirection(int x, int y) const override {
    return (abs(x) == 1 && y == 0) || (abs(y) == 1 && x == 0);
  }
};

#endif // SPY_H

