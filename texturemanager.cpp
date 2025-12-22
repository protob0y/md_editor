#include "texturemanager.h"

TextureManager::TextureManager(SDL_Renderer * rinstance){
    std::cout << "TextureManager is initing..." << std::endl;
    renderer = rinstance;
}

TextureManager::~TextureManager(){
    for(auto& pair : textureCache){
        SDL_DestroyTexture(pair.second);
    }
    textureCache.clear();
}

SDL_Texture * TextureManager::LoadTexture(const std::string& filePath){
    if (textureCache.find(filePath) != textureCache.end()) { // if filePath was found in textureCache
        return textureCache[filePath]; // then return the corresponding texture
    }

    // otherwise, if this file hasn't been loaded before
    SDL_Surface * tempSurface = IMG_Load(filePath.c_str());
    if(!tempSurface){
        std::cout << "Error loading " << filePath << ": " << IMG_GetError() << std::endl;
        return NULL;
    }

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if(!texture){
        std::cout << "Error creating texture " << filePath << ": " << SDL_GetError() << std::endl;
        return NULL;
    }

    textureCache[filePath] = texture;
    return texture;
}

void TextureManager::RenderTexture(SDL_Texture * texture){
    SDL_RenderCopy(renderer, texture, NULL, NULL); // fullscreen, for now. aspect ratio ignored.
}