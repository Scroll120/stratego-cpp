#ifndef NORMALPIECE_H
#define NORMALPIECE_H
#include "piece.h"

#include <stdlib.h>

class NormalPiece : public Piece {
public:
  NormalPiece(int Rank){this->rank = Rank;};
  bool canMove() const override{ return true; };
  int attack(Piece* other) override{return rank - other->getRank();};
  bool canMoveDirection(int x, int y) const override {
    return (abs(x) == 1 && y == 0) || (abs(y) == 1 && x == 0);
  }
};



#endif //NORMALPIECE_H
