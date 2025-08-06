#include <iostream>
#include <memory>
#include <SDL.h>
#include <string>
#include <vector>
#include <SDL2/SDL_ttf.h>

#ifdef TEST_FILES_PATH
        std::string base_path = TEST_FILES_PATH;
#else
std::string base_path = "fonts/";
#endif

int main(int argc, char *argv[]) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event e;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Rect r1{10, 10, 250, 250};
    SDL_Rect r2{450, 450, 230, 230};

    bool running = true;
    bool isDragging = false;
    int dragOffsetX = 0;
    int dragOffsetY = 0;
    SDL_Rect* draggedRect = nullptr;

    SDL_CreateWindowAndRenderer(1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer);

    const std::string font_path = base_path + "arial.ttf";
    TTF_Font *font = TTF_OpenFont(font_path.c_str(), 16);

    SDL_Color currentColor = {255, 255, 255, 255};
    SDL_Color textColor = {0, 0, 0, 255};

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_RIGHT:
                        std::cout << "right key was pressed\n";
                        r1.x += 50;
                        break;
                    case SDLK_LEFT:
                        r1.x -= 50;
                        break;
                    case SDLK_DOWN:
                        r1.y += 50;
                        break;
                    case SDLK_UP:
                        r1.y -= 50;
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                switch (e.key.keysym.sym) {
                    case SDLK_RIGHT:
                        std::cout << "right was released\n";
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    SDL_Point mousePos = {e.button.x, e.button.y};
                    if (SDL_PointInRect(&mousePos, &r1)) {
                        isDragging = true;
                        draggedRect = &r1;
                        dragOffsetX = e.button.x - r1.x;
                        dragOffsetY = e.button.y - r1.y;
                    } else if (SDL_PointInRect(&mousePos, &r2)) {
                        isDragging = true;
                        draggedRect = &r2;
                        dragOffsetX = e.button.x - r2.x;
                        dragOffsetY = e.button.y - r2.y;
                    }
                }
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    isDragging = false;
                    draggedRect = nullptr;
                }
            } else if (e.type == SDL_MOUSEMOTION) {
                if (isDragging && draggedRect != nullptr) {
                    draggedRect->x = e.motion.x - dragOffsetX;
                    draggedRect->y = e.motion.y - dragOffsetY;
                }
            } else if (e.type == SDL_MOUSEWHEEL) {
                if (e.wheel.y != 0) {
                    static int colorIndex = 0;
                    colorIndex = (colorIndex + 1) % 4;

                    switch (colorIndex) {
                        case 0: currentColor = {255, 0, 0, 255};
                            break;
                        case 1: currentColor = {0, 255, 0, 255};
                            break;
                        case 2: currentColor = {0, 0, 255, 255};
                            break;
                        case 3: currentColor = {255, 255, 0, 255};
                            break;
                        default: break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        SDL_RenderFillRect(renderer, &r1);

        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        SDL_RenderFillRect(renderer, &r2);

        std::string positionText1 = "Rect 1 position: (" + std::to_string(r1.x) + ", " + std::to_string(r1.y) + ")";
        std::string positionText2 = "Rect 2 position: (" + std::to_string(r2.x) + ", " + std::to_string(r2.y) + ")";

        SDL_Surface *textSurface1 = TTF_RenderText_Solid(font, positionText1.c_str(), textColor);
        SDL_Surface *textSurface2 = TTF_RenderText_Solid(font, positionText2.c_str(), textColor);

        SDL_Texture *textTexture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
        SDL_Rect textRect1 = {r1.x, r1.y + r1.w / 2, textSurface1->w, textSurface1->h};
        SDL_RenderCopy(renderer, textTexture1, nullptr, &textRect1);

        SDL_Texture *textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
        SDL_Rect textRect2 = {r2.x, r2.y + r2.w / 2, textSurface2->w, textSurface2->h};
        SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2);

        SDL_DestroyTexture(textTexture1);
        SDL_FreeSurface(textSurface1);

        SDL_DestroyTexture(textTexture2);
        SDL_FreeSurface(textSurface2);

        SDL_RenderPresent(renderer);
        // SDL_Delay(10);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
