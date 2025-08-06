#include "GameController.h"
#include <iostream>

GameController::GameController() : gameManager(nullptr), /*textureMgr(nullptr),*/ window(nullptr), renderer(nullptr) {
    if (!initSDL()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return;
    }

    // not yet initialized
    // textureMgr = new TextureMgr(renderer);
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
    //not yet initialized
    // if (!gameManager || !textureMgr) return;

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
                    int x = event.button.x;
                    int y = event.button.y;
                    // can't really do anything yet this is just an example what can be done here
                }
                break;
        }
    }
}

void GameController::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // just guessing here again just an example
    // gameManager->board->render(renderer, textureMgr);

    SDL_RenderPresent(renderer);
}

void GameController::cleanup() {
    delete gameManager;
    // delete textureMgr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

