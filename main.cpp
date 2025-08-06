#include <iostream>
#include <memory>
#include <SDL.h>
#include <string>
#include <vector>
#include <SDL2/SDL_ttf.h>

#include "src/GameController/GameController.h"

#ifdef TEST_FILES_PATH
        std::string base_path = TEST_FILES_PATH;
#else
std::string base_path = "fonts/";
#endif

int main(int argc, char *argv[]) {
    GameController gameController;
    gameController.run();

    std::cout << "Game exited successfully." << std::endl;

    return 0;
}
