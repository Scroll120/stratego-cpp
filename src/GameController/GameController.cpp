#include "GameController.h"
#include <iostream>
#include <unordered_map>



GameController::GameController()
    : gameManager(nullptr), textureMgr(nullptr), window(nullptr), renderer(nullptr),
      pieceSelected(false), selectedX(-1), selectedY(-1) {

    TTF_Init();
    font = TTF_OpenFont("fonts/arial.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
    }

    if (!initSDL()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return;
    }

    textureMgr = new TextureManager(renderer);
    textureMgr->loadTextures();

    gameManager = new GameManager();
}

GameController::~GameController() {
    cleanup();
}

void GameController::renderText(const std::string& message, int x, int y, SDL_Color color) {
    if (!font) return;

    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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
                    int mx = event.button.x;
                    int my = event.button.y;

                    // Only consider clicks on the board area (600x600)
                    if (mx >= 0 && mx < 600 && my >= 0 && my < 600) {
                        int boardX, boardY;
                        convertScreenToBoard(mx, my, boardX, boardY);

                        Piece* piece = gameManager->board->getPiece(boardX, boardY);
                        if (piece && piece->canMove() && piece->getOwnerId() == static_cast<int>(gameManager->currentPlayer->getId())) {
                            dragging = true;
                            dragFromX = boardX;
                            dragFromY = boardY;
                            dragMouseX = mx;
                            dragMouseY = my;
                        }
                    }
                }
                break;

            case SDL_MOUSEMOTION:
                if (dragging) {
                    dragMouseX = event.motion.x;
                    dragMouseY = event.motion.y;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (dragging) {
                        int dropX, dropY;
                        convertScreenToBoard(event.button.x, event.button.y, dropX, dropY);

                        if (gameManager->attemptMove(dragFromX, dragFromY, dropX, dropY)) {
                            gameManager->updateGameState();
                        }

                        dragging = false;
                        dragFromX = dragFromY = -1;
                    }
                }
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    if (dragging) {
                        dragging = false;
                        dragFromX = dragFromY = -1;
                    }
                }
                break;
        }
    }
}
void GameController::render() {
    SDL_RenderClear(renderer);

    // Draw board background texture (600x600 for board)
    SDL_Texture* boardTex = textureMgr->getTexture("board");
    if (boardTex) {
        SDL_Rect boardRect = {0, 0, 600, 600};
        SDL_RenderCopy(renderer, boardTex, nullptr, &boardRect);
    }
    renderBoard();
    renderPieces();

    if (pieceSelected) {
        SDL_Rect highlightRect = {selectedX * 60, selectedY * 60, 60, 60};
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 128);
        SDL_RenderFillRect(renderer, &highlightRect);
    }

    SDL_Color white = {255, 255, 255, 255};

    if (!gameManager->isRunning) {
        int winnerId = static_cast<int>(gameManager->currentPlayer->getId());
        std::string winMsg = (winnerId == 0) ? "Red Wins!" : "Blue Wins!";
        renderText(winMsg, 10, 10, white);  // Draw text at top-left instead of sidebar
    } else {
        int playerId = static_cast<int>(gameManager->currentPlayer->getId());
        std::string turnText = (playerId == 1) ? "Red's Turn" : "Blue's Turn";
        renderText(turnText, 600, 10, white);
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

std::string GameController::getTextureKeyForPiece(const Piece* piece) {
    static const std::unordered_map<int, std::string> rankToName = {
        {1, "spy"},
        {2, "scout"},
        {3, "miner"},
        {4, "sergeant"},
        {5, "lieutenant"},
        {6, "captain"},
        {7, "major"},
        {8, "colonel"},
        {9, "general"},
        {10, "marshal"},
        {11, "bomb"},
        {12, "flag"}
    };

    if (!piece) return "unknown";

    int rank = piece->getRank();
    int ownerId = piece->getOwnerId();
    std::string color = (ownerId == static_cast<int>(PlayerID::PLAYER1)) ? "red" : "blue";

    auto it = rankToName.find(rank);
    if (it != rankToName.end()) {
        return it->second + "_" + color;
    }
    return "unknown_" + color;
}

void GameController::renderPieces() {
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            if (dragging && x == dragFromX && y == dragFromY) continue;

            const Piece* piece = gameManager->board->getPiece(x, y);
            if (!piece) continue;

            SDL_Rect pieceRect = {x * 60 + 5, y * 60 + 5, 50, 50};

            int ownerId = piece->getOwnerId();
            int currentPlayerId = static_cast<int>(gameManager->currentPlayer->getId());

            // Reveal all blue pieces if it's blue's turn,
            // else reveal only red pieces and hide blue pieces
            bool reveal = false;
            if (currentPlayerId == static_cast<int>(PlayerID::PLAYER2)) {
                // Blue's turn: reveal all blue pieces, hide red pieces
                reveal = (ownerId == static_cast<int>(PlayerID::PLAYER2));
            } else {
                // Red's turn: reveal red pieces, hide blue pieces
                reveal = (ownerId == static_cast<int>(PlayerID::PLAYER1));
            }

            if (reveal || piece->getIsRevealed()) {
                std::string textureKey = getTextureKeyForPiece(piece);
                SDL_Texture* tex = textureMgr->getTexture(textureKey);
                if (tex) {
                    SDL_RenderCopy(renderer, tex, nullptr, &pieceRect);
                } else {
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderFillRect(renderer, &pieceRect);
                }
            } else {
                // Render hidden piece backside texture
                std::string backKey = (ownerId == static_cast<int>(PlayerID::PLAYER1)) ? "red_back" : "blue_back";
                SDL_Texture* backTex = textureMgr->getTexture(backKey);
                if (backTex) {
                    SDL_RenderCopy(renderer, backTex, nullptr, &pieceRect);
                } else {
                    // Fallback colored rectangle
                    SDL_SetRenderDrawColor(renderer,
                        (ownerId == static_cast<int>(PlayerID::PLAYER1)) ? 200 : 50,
                        0,
                        (ownerId == static_cast<int>(PlayerID::PLAYER1)) ? 50 : 200,
                        255);
                    SDL_RenderFillRect(renderer, &pieceRect);
                }
            }
        }
    }

    // Render dragged piece under cursor
    if (dragging) {
        const Piece* draggedPiece = gameManager->board->getPiece(dragFromX, dragFromY);
        if (draggedPiece) {
            SDL_Rect dragRect = {dragMouseX - 25, dragMouseY - 25, 50, 50};
            std::string textureKey = getTextureKeyForPiece(draggedPiece);
            SDL_Texture* tex = textureMgr->getTexture(textureKey);
            if (tex) {
                SDL_RenderCopy(renderer, tex, nullptr, &dragRect);
            } else {
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &dragRect);
            }
        }
    }
}

void GameController::cleanup() {
    delete gameManager;
    delete textureMgr;

    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
