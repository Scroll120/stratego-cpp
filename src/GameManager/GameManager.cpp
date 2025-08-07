#include "GameManager.h"
#include "../pieces/flag.h"
#include "../pieces/bomb.h"
#include "../pieces/spy.h"
#include "../pieces/scout.h"
#include "../pieces/miner.h"
#include "../pieces/normalPiece.h"

#include <SDL_log.h>
#include <SDL_timer.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <stdexcept>

GameManager::GameManager() : player1(nullptr), player2(nullptr), currentPlayer(nullptr), board(nullptr),
                             isRunning(true), gameInitialized(false) {
    initializeGame();
}

Piece* GameManager::createPieceByRank(int rank) {
    switch (rank) {
    case 1: return new Spy();
    case 2: return new Scout();
    case 3: return new Miner();
    case 4: return new NormalPiece(4);
    case 5: return new NormalPiece(5);
    case 6: return new NormalPiece(6);
    case 7: return new NormalPiece(7);
    case 8: return new NormalPiece(8);
    case 9: return new NormalPiece(9);
    case 10: return new NormalPiece(10);
    case 11: return new Bomb();
    case 12: return new Flag();
    default: return nullptr;
    }
}

void GameManager::placePiecesForPlayer(PlayerID id, int startRow, int endRow) {
    Player* player = (id == PlayerID::PLAYER1) ? player1 : player2;

    std::vector<std::pair<int, int>> positions;
    for (int y = startRow; y <= endRow; ++y) {
        for (int x = 0; x < 10; ++x) {
            positions.emplace_back(x, y);
        }
    }

    std::shuffle(positions.begin(), positions.end(), std::default_random_engine(SDL_GetTicks()));

    std::map<int, int> pieceCounts = {
        {1, 1}, {2, 8}, {3, 5}, {4, 4}, {5, 4}, {6, 4},
        {7, 3}, {8, 2}, {9, 1}, {10, 1}, {11, 6}, {12, 1}
    };

    int posIndex = 0;
    for (const auto& [rank, count] : pieceCounts) {
        for (int i = 0; i < count; ++i) {
            if (posIndex >= positions.size()) {
                SDL_Log("Warning: Not enough free positions to place all pieces for player %d", static_cast<int>(id));
                return;
            }

            auto [x, y] = positions[posIndex++];
            Piece* p = createPieceByRank(rank);
            p->setOwnerId(static_cast<int>(id));
            board->placePiece(x, y, p);
            player->addPiece(p);
        }
    }
}

GameManager::~GameManager() {
    delete player1;
    delete player2;
    delete board;
}

void GameManager::initializeGame() {
    player1 = new Player(PlayerID::PLAYER1);
    player2 = new Player(PlayerID::PLAYER2);

    board = new Board();
    setupBoard();

    placePiecesForPlayer(PlayerID::PLAYER1, 6, 9); // Red side rows
    placePiecesForPlayer(PlayerID::PLAYER2, 0, 3); // Blue side rows

    currentPlayer = player2; // Blue starts
    isRunning = true;
    setupPhase = false;
}

void GameManager::setupBoard() {
    board->addObstacle(2, 4);
    board->addObstacle(3, 4);
    board->addObstacle(2, 5);
    board->addObstacle(3, 5);
    board->addObstacle(6, 4);
    board->addObstacle(7, 4);
    board->addObstacle(6, 5);
    board->addObstacle(7, 5);
}

bool GameManager::checkWin() {
    if (currentPlayer == player1 && player2->isFlagCaptured()) {
        SDL_Log("Player 2 flag captured - Player 1 wins");
        return true;
    }
    if (currentPlayer == player2 && player1->isFlagCaptured()) {
        SDL_Log("Player 1 flag captured - Player 2 wins");
        return true;
    }
    if (currentPlayer == player1 && !player2->hasMoveablePiece()) {
        SDL_Log("Player 2 has no movable pieces - Player 1 wins");
        return true;
    }
    if (currentPlayer == player2 && !player1->hasMoveablePiece()) {
        SDL_Log("Player 1 has no movable pieces - Player 2 wins");
        return true;
    }
    return false;
}

void GameManager::updateGameState() {
    if (checkWin()) {
        SDL_Log("Game over detected in updateGameState");
        isRunning = false;
        return;
    }
}

void GameManager::switchPlayer() {
    currentPlayer = (currentPlayer == player1) ? player2 : player1;
}

bool GameManager::validateMove(int fromX, int fromY, int toX, int toY) {
    if (fromX < 0 || fromX >= 10 || fromY < 0 || fromY >= 10 ||
        toX < 0 || toX >= 10 || toY < 0 || toY >= 10) {
        return false;
    }

    Piece* piece = board->getPiece(fromX, fromY);
    if (!piece || (piece->getRank() == 11 || piece->getRank() == 12)) {
        return false;
    }

    try {
        return board->isMoveValid(fromX, fromY, toX, toY);
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception in isMoveValid: " << e.what() << std::endl;
        return false;
    }
}

bool GameManager::attemptMove(int fromX, int fromY, int toX, int toY) {
    if (!validateMove(fromX, fromY, toX, toY)) {
        return false;
    }

    auto moveResult = board->movePiece(fromX, fromY, toX, toY);
    if (moveResult.success && moveResult.attackerDead == nullptr) {
        Piece* attacker = board->getPiece(toX, toY);
        if (attacker) {
            attacker->setIsRevealed(false);
        }
    }
    if (!moveResult.success) {
        return false;
    }

    // Remove and delete dead pieces for attacker
    if (moveResult.attackerDead) {
        Player* owner = (moveResult.attackerDead->getOwnerId() == static_cast<int>(PlayerID::PLAYER1)) ? player1 : player2;
        owner->removePiece(moveResult.attackerDead);
        delete moveResult.attackerDead;
    }

    // Remove and delete dead pieces for defender
    if (moveResult.defenderDead) {
        Player* owner = (moveResult.defenderDead->getOwnerId() == static_cast<int>(PlayerID::PLAYER1)) ? player1 : player2;
        owner->removePiece(moveResult.defenderDead);
        delete moveResult.defenderDead;
    }

    switchPlayer();

    return true;
}
void GameManager::endTurn() {
    // Switch current player
    currentPlayer = (currentPlayer == player1) ? player2 : player1;

    PlayerID currentId = currentPlayer->getId();

    // Hide enemy pieces only, keep current player's pieces' revealed state
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            Piece* piece = board->getPiece(x, y);
            if (!piece) continue;

            if (piece->getOwnerId() != static_cast<int>(currentId)) {
                piece->setIsRevealed(false);
            }
        }
    }
}

