// textureManager is instanciated only ONCE, by main.cpp
// it assumes that IMG_Init() was already called and errors were handeled.
// it then can load images and render it to the screen directly, given a renderer.
// its instance is passed (by main) to other modules which need to load and draw images.

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <map>

class TextureManager{
    public:
    TextureManager(SDL_Renderer * rinstance);
    ~TextureManager();

    SDL_Texture * LoadTexture(const std::string& filePath);
    void RenderTexture(SDL_Texture * texture);

    private:
    SDL_Renderer * renderer;
    std::map<std::string, SDL_Texture*> textureCache; 
};

#endif