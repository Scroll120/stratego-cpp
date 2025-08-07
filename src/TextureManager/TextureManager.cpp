#include "TextureManager.h"
#include <SDL_image.h>
#include <iostream>

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
        {"board",      "textures/DungeonMapDoodle.png"},
        {"scout_red",  "textures/scout.jpeg"},
        {"scout_blue", "textures/scout.jpeg"},
        {"miner_red",  "textures/miner.jpeg"},
        {"miner_blue", "textures/miner.jpeg"},
        {"bomb_red",   "textures/bomb.jpeg"},
        {"bomb_blue",  "textures/bomb.jpeg"},
        {"flag_red",   "textures/flag.jpeg"},
        {"flag_blue",  "textures/flag.jpeg"},
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
        std::cout << "[TextureManager] Loaded texture: " << key << " from " << path << "\n";
    }
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