//font.h handles TTF rendering and glyph extracting. It can render one letter to the screen.

#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>

class Font{
public:
    Font(SDL_Renderer * renderer, const std::string& fontPath, int fontSize); //constructor
    ~Font(); //destructor

    void setColor(int r, int g, int b);
    void renderTable(SDL_Renderer * renderer, int x, int y);
    void renderChar(SDL_Renderer * renderer, char32_t letter, int x, int y);
    int getGlyphWidth();
    int getGlyphHeight();
private:
    TTF_Font* ttfFont = NULL;
    int atlasSizeX = 512, atlasSizeY = 512;
    std::map<char32_t, SDL_Rect> glyphRects; // map is something like a dictionary in python
    SDL_Surface * atlasSurface = NULL;
    SDL_Texture * textureAtlas = NULL;
    int draw_x = 0; // position where new glyphs will be added
    int draw_y = 0;

    void AddGlyphToSurface(char32_t c);
};



#endif