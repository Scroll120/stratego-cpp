#ifndef PIECE_H
#define PIECE_H

class Piece {
public:
  virtual ~Piece() = default;

  virtual bool canMove() const = 0;
  virtual bool canMoveDirection(int x, int y) const = 0;
  virtual int attack(Piece* other) = 0;

  int getRank() const { return rank; }
  bool getIsRevealed() const { return isRevealed; }
  bool getIsDragged() const { return isDragged; }

  void reveal() { isRevealed = true; }
  void setDragged(bool dragged) { isDragged = dragged; }

protected:
  int rank = 0;
  bool isRevealed = false;
  bool isDragged = false;
};




#endif //PIECE_H
