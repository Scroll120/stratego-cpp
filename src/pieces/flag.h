#ifndef FLAG_H
#define FLAG_H
#include "piece.h"

class flag : public Piece {
  public:
  flag(){ rank = 12;};
  bool canMove() const override{ return false; };
  int attack(Piece *other) override { return -1; };
};



#endif //FLAG_H
