#include "Player.h"
#include "../pieces/Piece.h"

bool Player::addPiece(Piece *piece) {
  if (getPieces().size() == getMaxSize()) {
    return false;
  }
  pieces.push_back(piece);
  return true;
}

bool Player::removePiece(Piece *piece) {
    for (auto it = getPieces().begin(); it != getPieces().end(); it++) {
      if (*it == piece) {
        pieces.erase(it);
        return true;
      }
    }
  return false;
}

bool Player::isFlagCaptured() {
  for (auto it = getPieces().begin(); it != getPieces().end(); it++) {
    if ((*it)->getRank() == 12) {
      return false;
    }
  }
  return true;
}

bool Player::hasMoveablePiece() {
  for (auto it = getPieces().begin(); it != getPieces().end(); it++) {
    if ((*it)->getRank() != 12 && (*it)->getRank() != 11) {
      return true;
    }
  }
  return false;
}
