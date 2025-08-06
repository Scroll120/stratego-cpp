#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include <SDL.h>
#include  "../GameManager/GameManager.h"


class GameController {
public:
    GameController();

    ~GameController();

    void run();

    void handleEvents();

    void render();

private:
    GameManager* gameManager;
    // TextureMgr* textureMgr;
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool initSDL();

    void cleanup();
};


#endif //GAMECONTROLLER_H
