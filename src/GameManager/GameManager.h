#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "../game/Board.h"
#include "../game/Player.h"


class GameManager {
public:
    GameManager();
    ~GameManager();

    bool checkWin();
    void updateGameState();
    void switchPlayer();
    bool validateMove(int fromX, int fromY, int toX, int toY);
    bool attemptMove(int fromX, int fromY, int toX, int toY);

    Player* currentPlayer;
    Board* board;
    bool isRunning;

private:
    Player* player1;
    Player* player2;
    bool gameInitialized;

    void initializeGame();
    void setupPiecesForPlayer(Player* player, bool isPlayer1);
    void setupBoard();
};


#endif //GAMEMANAGER_H
