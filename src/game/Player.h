#ifndef PLAYER_H
#define PLAYER_H
#include <vector>

enum class PlayerID { PLAYER1 = 1, PLAYER2 = 2 };

class Piece;
class Player {
private:
  PlayerID id;
  std::vector<Piece *> pieces;
  std::vector<Piece*> reservePieces;
  static constexpr int MAX_PIECES = 40;
public:
  Player(PlayerID id) : id(id) {}
  PlayerID getId() const { return id; }
  bool addPiece(Piece* piece);
  const std::vector<Piece*>& getPieces() const { return pieces; }
  std::vector<Piece*>& getReservePieces() {
    return reservePieces;
  }
  void addPieceToReserve(Piece* p) { reservePieces.push_back(p); }
  int getMaxSize() const { return MAX_PIECES; };
  bool removePiece(Piece* piece);
  bool isFlagCaptured();
  bool hasMoveablePiece();
};

#endif //PLAYER_H
