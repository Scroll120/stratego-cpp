#include "Player.h"
#include "../pieces/Piece.h"

bool Player::addPiece(Piece* piece) {
  if ((int)pieces.size() >= getMaxSize()) {
    return false;
  }
  pieces.push_back(piece);
  return true;
}

bool Player::removePiece(Piece* piece) {
  for (auto it = pieces.begin(); it != pieces.end(); ++it) {
    if (*it == piece) {
      pieces.erase(it);
      return true;
    }
  }
  return false;
}

bool Player::isFlagCaptured() {
  for (Piece* p : pieces) {
    if (p && p->getRank() == 12) {
      return false;
    }
  }
  return true;
}

bool Player::hasMoveablePiece() {
  for (Piece* p : pieces) {
    if (p && p->canMove()) {
      return true;
    }
  }
  return false;
}
