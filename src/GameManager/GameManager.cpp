#include "GameManager.h"
#include "../pieces/flag.h"
#include "../pieces/bomb.h"
#include "../pieces/spy.h"
#include "../pieces/scout.h"
#include "../pieces/miner.h"
#include "../pieces/normalPiece.h"

GameManager::GameManager() : player1(nullptr), player2(nullptr), currentPlayer(nullptr), board(nullptr),
                             isRunning(true), gameInitialized(false) {
    initializeGame();
}

GameManager::~GameManager() {
    delete player1;
    delete player2;
    delete board;
}

void GameManager::initializeGame() {
    if (gameInitialized) return;

    player1 = new Player(PlayerID::PLAYER1);
    player2 = new Player(PlayerID::PLAYER2);
    currentPlayer = player1;

    board = new Board();
    setupBoard();

    setupPiecesForPlayer(player1, true);
    setupPiecesForPlayer(player2, false);

    gameInitialized = true;
}

void GameManager::setupPiecesForPlayer(Player *player, bool isPlayer1) {
    player->addPiece(new Flag());

    for (int i = 0; i < 6; i++) {
        player->addPiece(new Bomb());
    }

    player->addPiece(new Spy());

    for (int i = 0; i < 8; i++) {
        player->addPiece(new scout);
    }

    for (int i = 0; i < 5; i++) {
        player->addPiece(new Miner());
    }

    int normalPieceCounts[] = {4, 4, 4, 3, 2, 1};
    for (int rank = 4; rank <= 9; rank++) {
        for (int i = 0; i < normalPieceCounts[rank - 4]; i++) {
            player->addPiece(new NormalPiece(rank));
        }
    }
}

void GameManager::setupBoard() {
    board->addObstacle(4, 2);
    board->addObstacle(4, 3);
    board->addObstacle(4, 6);
    board->addObstacle(4, 7);
    board->addObstacle(5, 2);
    board->addObstacle(5, 3);
    board->addObstacle(5, 6);
    board->addObstacle(5, 7);
}

bool GameManager::checkWin() {
    if (currentPlayer == player1 && player2->isFlagCaptured()) {
        return true;
    }
    if (currentPlayer == player2 && player1->isFlagCaptured()) {
        return true;
    }

    if (currentPlayer == player1 && !player2->hasMoveablePiece()) {
        return true;
    }
    if (currentPlayer == player2 && !player1->hasMoveablePiece()) {
        return true;
    }

    return false;
}

void GameManager::updateGameState() {
    if (checkWin()) {
        isRunning = false;
        return;
    }
}

void GameManager::switchPlayer() {
    currentPlayer = (currentPlayer == player1) ? player2 : player1;
}

bool GameManager::validateMove(int fromX, int fromY, int toX, int toY) {
    if (fromX < 0 || fromX >= 10 || fromY < 0 || fromY >= 10 || toX < 0 || toX >= 10 || toY < 0 || toY >= 10) {
        return false;
    }

    Piece* piece = board->getPiece(fromX, fromY);
    if (!piece || (piece->getRank() == 11 || piece->getRank() == 12)) {
        return false;
    }

    return board->isMoveValid(fromX, fromY, toX, toY);
}

bool GameManager::attemptMove(int fromX, int fromY, int toX, int toY) {
    if (!validateMove(fromX, fromY, toX, toY)) {
        return false;
    }

    bool moveSuccess = board->movePiece(fromX, fromY, toX, toY);
    if (moveSuccess) {
        switchPlayer();
    }

    return moveSuccess;
}
