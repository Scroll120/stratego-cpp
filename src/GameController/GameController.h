#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "../GameManager/GameManager.h"
#include "../TextureManager/TextureManager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class GameController {
public:

    GameController();
    ~GameController();

    void run();
    void handleEvents();
    void renderSidebar();
    void render();
    std::string getTextureKeyForPiece(const Piece* piece);
    void renderText(const std::string& message, int x, int y, SDL_Color color);

private:
    bool dragging = false;
    int dragFromX = -1, dragFromY = -1;
    int dragMouseX = 0, dragMouseY = 0;

    bool draggingReservePiece = false;
    Piece* draggedPiece = nullptr;
    int dragPosX = 0;
    int dragPosY = 0;

    TTF_Font * font;

    GameManager* gameManager;
    TextureManager* textureMgr;

    SDL_Window* window;
    SDL_Renderer* renderer;

    bool pieceSelected;
    int selectedX;
    int selectedY;

    bool initSDL();
    void cleanup();
    void convertScreenToBoard(int screenX, int screenY, int& boardX, int& boardY);
    void renderBoard();
    void renderPieces();
};

#endif // GAMECONTROLLER_H
