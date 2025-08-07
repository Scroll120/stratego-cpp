#ifndef FLAG_H
#define FLAG_H
#include "piece.h"

class Flag : public Piece {
  public:
  Flag(){ rank = 12;};
  bool canMove() const override{ return false; };
  bool canMoveDirection(int x, int y) const override {return false;};
  int attack(Piece *other) override { return false; };
};

#endif //FLAG_H
