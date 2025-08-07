#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "../game/Board.h"
#include "../game/Player.h"

class Piece;

class GameManager {
public:
    GameManager();
  void placePiecesForPlayer(PlayerID id, int startRow, int endRow);
  ~GameManager();

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    Piece* createPieceByRank(int rank);

    bool checkWin();
    void updateGameState();
    void switchPlayer();
    bool validateMove(int fromX, int fromY, int toX, int toY);
    bool attemptMove(int fromX, int fromY, int toX, int toY);
    void endTurn();
    bool isSetupPhase() const { return setupPhase; }

    Player* currentPlayer;
    Board* board;
    bool isRunning;

private:
    bool setupPhase = true;
    bool gameInitialized = false;

    Player* player1;
    Player* player2;

    void initializeGame();
    void setupBoard();
};

#endif // GAMEMANAGER_H

