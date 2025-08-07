#include "TextureManager.h"
#include <SDL_image.h>
#include <iostream>
#include <vector>

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer(renderer) {}

TextureManager::~TextureManager() {
    cleanup();
}

void TextureManager::loadTextures() {
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "[TextureManager] IMG_Init failed: " << IMG_GetError() << "\n";
        return;
    }

    const std::vector<std::pair<std::string, std::string>> assetList = {
        {"board",      "textures/raw.png"},

        {"flag_red",   "textures/RF.png"},
        {"flag_blue",  "textures/BF.png"},

        {"bomb_red",   "textures/RB.png"},
        {"bomb_blue",  "textures/BB.png"},

        {"scout_red",  "textures/R02.png"},
        {"scout_blue", "textures/B02.png"},

        {"spy_red",    "textures/R01.png"},
        {"spy_blue",   "textures/B01.png"},

        {"miner_red",  "textures/R03.png"},
        {"miner_blue", "textures/B03.png"},

        {"sergeant_red", "textures/R04.png"},
        {"sergeant_blue","textures/B04.png"},

        {"lieutenant_red", "textures/R05.png"},
        {"lieutenant_blue","textures/B05.png"},

       {"captain_red", "textures/R06.png"},
        {"captain_blue","textures/B06.png"},

        {"major_red", "textures/R07.png"},
        {"major_blue","textures/B07.png"},

        {"colonel_red", "textures/R08.png"},
        {"colonel_blue","textures/B08.png"},

        {"general_red", "textures/R09.png"},
        {"general_blue","textures/B09.png"},

        {"marshal_red", "textures/R10.png"},
        {"marshal_blue","textures/B10.png"},

        {"red_back", "textures/R00.png"},
        {"blue_back", "textures/B00.png"},

    };

    for (const auto& [key, path] : assetList) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "[TextureManager] Failed to load " << path
                      << " for key '" << key << "': " << IMG_GetError() << "\n";
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            std::cerr << "[TextureManager] Failed to create texture for key '"
                      << key << "': " << SDL_GetError() << "\n";
            continue;
        }

        textures[key] = texture;
        std::cout << "[TextureManager] Loaded texture: " << key << "\n"<< std::endl << std::flush;;

    }
    std::cout << "[TextureManager] Loaded " << textures.size() << " textures total.\n"<< std::endl << std::flush;;
}

SDL_Texture* TextureManager::getTexture(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    std::cerr << "[TextureManager] Texture not found: " << name << "\n";
    return nullptr;
}

void TextureManager::cleanup() {
    for (auto& [key, tex] : textures) {
        SDL_DestroyTexture(tex);
    }
    textures.clear();
}