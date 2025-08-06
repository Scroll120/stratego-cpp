#include "GameController.h"
#include <iostream>

GameController::GameController() : gameManager(nullptr), window(nullptr), renderer(nullptr), pieceSelected(false),
                                   selectedX(-1), selectedY(-1) {
    if (!initSDL()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return;
    }

    gameManager = new GameManager();
}

GameController::~GameController() {
    cleanup();
}

bool GameController::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Stratego", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void GameController::run() {
    if (!gameManager) return;

    Uint32 frameStart;
    int frameTime;
    constexpr int FPS = 180;
    constexpr int frameDelay = 1000 / FPS;

    while (gameManager->isRunning) {
        frameStart = SDL_GetTicks();

        handleEvents();
        gameManager->updateGameState();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void GameController::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                gameManager->isRunning = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int boardX, boardY;
                    convertScreenToBoard(event.button.x, event.button.y, boardX, boardY);

                    if (boardX >= 0 && boardY < 10 && boardY >= 0 && boardY < 10) {
                        if (!pieceSelected) {
                            Piece *piece = gameManager->board->getPiece(boardX, boardY);
                            if (piece && piece->canMove()) {
                                pieceSelected = true;
                                selectedX = boardX;
                                selectedY = boardY;
                            }
                        } else {
                            if (gameManager->attemptMove(selectedX, selectedY, boardX, boardY)) {
                                gameManager->updateGameState();
                            }
                            pieceSelected = false;
                        }
                    }
                }
                break;;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    pieceSelected = false;
                }
                break;
        }
    }
}

void GameController::render() {
    SDL_Rect rightPanel = {600, 0, 200, 600};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &rightPanel);

    renderBoard();
    renderPieces();

    if (pieceSelected) {
        SDL_Rect highlightRect = {selectedX * 60, selectedY * 60, 60, 60};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 128);
        SDL_RenderFillRect(renderer, &highlightRect);
    }

    SDL_RenderPresent(renderer);
}

void GameController::convertScreenToBoard(int screenX, int screenY, int &boardX, int &boardY) {
    // assuming 600x600 board area (60x60 per cell)
    boardX = screenX / 60;
    boardY = screenY / 60;
}

void GameController::renderBoard() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= 10; i++) {
        SDL_RenderDrawLine(renderer, i * 60, 0, i * 60, 600);
        SDL_RenderDrawLine(renderer, 0, i * 60, 600, i * 60);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (const auto &obs: gameManager->board->getObstacles()) {
        SDL_Rect lakeRect = {obs.x * 60, obs.y * 60, 60, 60};
        SDL_RenderFillRect(renderer, &lakeRect);
    }
}

void GameController::renderPieces() {
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            if (const Piece *piece = gameManager->board->getPiece(x, y)) {
                SDL_Rect pieceRect = {x * 60 + 5, y * 60 + 5, 50, 50};

                if (piece->getRank() == 12) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                } else if (piece->getRank() == 11) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                } else {
                    bool isPlayer1Piece = y < 4;
                    SDL_SetRenderDrawColor(renderer,
                                           isPlayer1Piece ? 0 : 255,
                                           0,
                                           isPlayer1Piece ? 255 : 0,
                                           255);
                }

                SDL_RenderFillRect(renderer, &pieceRect);

                if (piece->getIsRevealed() && piece->getRank() != 11 && piece->getRank() != 12) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawLine(renderer, x * 60 + 30, y * 60 + 25, x * 60 + 30, y * 60 + 35);
                }
            }
        }
    }
}


void GameController::cleanup() {
    delete gameManager;
    // delete textureMgr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
