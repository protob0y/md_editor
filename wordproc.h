// wordProcessor handles button presses such as return, arrow keys, or text input.
// it contains the whole document. It can render relevant areas to the screen, given a renderer in the constructor.

#ifndef WORDPROC_H
#define WORDPROC_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "font.h"
#include <string>
#include <vector>
#include <fstream>
#include "texturemanager.h"
#include "dialog.h"
#include "fileio.h"

#include "utf8.h"

class WordProc{
    public:
    WordProc(SDL_Renderer * rinstance, Font * finstance, TextureManager * tminstance, Dialog * dinstance);
    void RenderScreen();
    
    void RegisterTextinput(const char * utf8Text);
    void RegisterKeypress(SDL_Event * event);


    private:
    Font * smallfont = NULL; // instanced by main
    SDL_Renderer * renderer = NULL;
    TextureManager * texMan = NULL;
    Dialog * saveDialog = NULL;

    SDL_Texture * background = NULL;

    std::vector<std::u32string> document;

    int cursor_line = 0; // is zero in first line of document
    int cursor_col = 0; // cursor col counts behind last glyph (i.e. "Hi|" <-- cursor_col = 2)
    const int cursorBlinkInterval = 1000;
    Uint32 cursorBlinkStart;
    bool cursorBlinkShowing = true;

    bool showSaveDialog = false;

    int margin_left = 20;
    int margin_top = 20;
    int addLineSpacing = 5;

    void DrawCursor();
};



#endif