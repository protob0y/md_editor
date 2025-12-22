#include "font.h"
#include <iostream>

Font::Font(SDL_Renderer * renderer,  const std::string& fontPath, int fontSize){
    // init ttf
    if (TTF_Init() == -1) {
        std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        textureAtlas = nullptr;
        return;
    }

    // load ttf file
    ttfFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!ttfFont) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        textureAtlas = nullptr;
        return;
    }

    std::u32string chars = U"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    U"1234567890ÄÖÜäöüß.,:!? +-*/()#^<>"; // later all these will be rendered to the atlas

    // atlasTexture will later be rendered from atlasSurface
    atlasSurface = SDL_CreateRGBSurfaceWithFormat(0, atlasSizeX, atlasSizeY, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_FillRect(atlasSurface, NULL, SDL_MapRGBA(atlasSurface->format, 0, 0, 0, 0)); // transparency

    for(char32_t c : chars){
        AddGlyphToSurface(c);
    }

    textureAtlas = SDL_CreateTextureFromSurface(renderer, atlasSurface);
}

Font::~Font(){
    TTF_CloseFont(ttfFont);
    if(textureAtlas){
        SDL_DestroyTexture(textureAtlas);
    }
    if(atlasSurface){
        SDL_FreeSurface(atlasSurface);
    }
    TTF_Quit();
}

void Font::setColor(int r, int g, int b){
    SDL_SetTextureColorMod(textureAtlas, r, g, b);
}

void Font::renderTable(SDL_Renderer * renderer, int x, int y){
    //Blits texture atlas to screen
    SDL_Rect srcRect = {0, 0, 512, 512};
    SDL_Rect dstRect = {x, y, srcRect.w, srcRect.h};
    SDL_RenderCopy(renderer, textureAtlas, &srcRect, &dstRect);
}

void Font::renderChar(SDL_Renderer * renderer, char32_t letter, int x, int y){
    //Blits given character to given position on screen
    if(glyphRects.find(letter) == glyphRects.end()){
        std::cout << "Trying to render a newly requested letter. This shouldnt happen too often." << std::endl;
        AddGlyphToSurface(letter);
        textureAtlas = SDL_CreateTextureFromSurface(renderer, atlasSurface);
        return;
    }
    
    SDL_Rect srcRect = glyphRects[letter];
    SDL_Rect dstRect = {x, y, srcRect.w, srcRect.h};
    SDL_RenderCopy(renderer, textureAtlas, &srcRect, &dstRect);
}

int Font::getGlyphWidth(){
    return glyphRects.begin()->second.w; // returns the width of the first value in the map
}

int Font::getGlyphHeight(){
    return glyphRects.begin()->second.h;
}

void Font::AddGlyphToSurface(char32_t c){ // adds a glyph to the atlas Surface, does upate texture!
    SDL_Surface *glyphSurface = TTF_RenderGlyph32_Blended(ttfFont, c, {255, 255, 255, 255});
    if(!glyphSurface){
        std::cout << "Error rendering a glyph." << std::endl;
        return;
    }
    //check if we need a new line for this glyph
    if(draw_x + glyphSurface->w > atlasSizeX){
        draw_x = 0;
        draw_y += glyphSurface->h + 1;
    }
    SDL_Rect destRect = {draw_x, draw_y, glyphSurface->w, glyphSurface->h};
    SDL_BlitSurface(glyphSurface, NULL, atlasSurface, &destRect); // copy glyph to atlas surface

    // save position in map
    glyphRects[c] = destRect;

    draw_x += glyphSurface->w;

    SDL_FreeSurface(glyphSurface);
}