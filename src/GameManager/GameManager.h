#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H


class GameManager {
public:
    GameManager();

    ~GameManager();

    bool checkWin();

    void updateGameState();

    void switchPlayer();

    // doesn't exist yet
    // Player* currentPlayer;
    // Board* board;
    bool isRunning;

private:
    // doesn't exist yet
    // Player* player1;
    // Player* player2;

    void initializeGame();

    bool validateMove(/* move parameters */);
};


#endif //GAMEMANAGER_H
