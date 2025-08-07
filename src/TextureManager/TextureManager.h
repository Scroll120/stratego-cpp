
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#pragma once

#include <string>
#include <unordered_map>
#include <SDL.h>

class TextureManager {
private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textures;

public:
    explicit TextureManager(SDL_Renderer* renderer);
    ~TextureManager();

    void loadTextures();
    SDL_Texture* getTexture(const std::string& name);
    void cleanup();
};



#endif //TEXTUREMANAGER_H
