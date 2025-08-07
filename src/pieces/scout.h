#ifndef SCOUT_H
#define SCOUT_H
#include "piece.h"

class Scout : public Piece {
public:
  Scout() { rank = 2;};
  bool canMove() const override { return true; };
  int attack(Piece *other) override{ return rank - other->getRank();};
  bool canMoveDirection(int x, int y) const override {
    return (x == 0 && y != 0) || (y == 0 && x != 0);
  }
};



#endif //SCOUT_H
