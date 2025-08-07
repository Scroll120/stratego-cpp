#include "TextureManager.h"
#include <SDL_image.h>
#include <iostream>

TextureManager::TextureManager(SDL_Renderer* renderer) : renderer(renderer) {}

TextureManager::~TextureManager() {
    cleanup();
}

void TextureManager::loadTextures() {
    const std::vector<std::pair<std::string, std::string>> assetList = {
        {"board",      "assets/board.png"},

        {"scout_red",  "assets/pieces/red/scout.png"},
        {"scout_blue", "assets/pieces/blue/scout.png"},
        {"miner_red",  "assets/pieces/red/miner.png"},
        {"miner_blue", "assets/pieces/blue/miner.png"},
        {"bomb_red",   "assets/pieces/red/bomb.png"},
        {"bomb_blue",  "assets/pieces/blue/bomb.png"},
        {"flag_red",   "assets/pieces/red/flag.png"},
        {"flag_blue",  "assets/pieces/blue/flag.png"},
    };

    for (const auto& [key, path] : assetList) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "[TextureManager] Failed to load " << path << ": " << IMG_GetError() << "\n";
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            std::cerr << "[TextureManager] Failed to create texture for " << key << ": " << SDL_GetError() << "\n";
            continue;
        }

        textures[key] = texture;
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