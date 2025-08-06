#include "GameManager.h"
#include <iostream>

GameManager::GameManager() : /*currentPlayer(nullptr), board(nullptr)*/isRunning(true)/*, player1(nullptr), player2(nullptr)*/ {
    initializeGame();
}

GameManager::~GameManager() {
    // delete player1;
    // delete player2;
    // delete board;
}

void GameManager::initializeGame() {
    // player1 = new Player();
    // player2 = new Player();

    // currentPlayer = player1;

    // board = new Board();
    // board->initialize();  I gues this will be the method??
}

bool GameManager::checkWin() {
    //check if the opponent's flag has been captured
    //or if all movable piecas are captured
    // bool player1Wins = somelogic;
    // bool player2Wins = somelogic;

    if (true/*player1Wins || player2Wins*/) {
        isRunning = false;
        return true;
    }
    return false;
}

void GameManager::updateGameState() {
    if (checkWin()) {
        return;
    }
    //update other game states
}

void GameManager::switchPlayer() {
    // currentPlayer = (currentPlayer == player1) ? player2 : player1;
}

bool GameManager::validateMove(/*move parameters*/) {
    //validate if the move is legal
    //check if the piece belongs to current player
    //check if destination is valid
    return true; //placeholder just like everyting else
}





