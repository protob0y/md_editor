//this class is instantiated by wordproc and gets called when the user presses the esc key

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "font.h"


#ifndef DIALOG_H
#define DIALOG_H

class Dialog{
    public:
    Dialog(SDL_Renderer * rinstance, Font * finstance);
    void RenderDialog();
    // TODO RegisterKeypress: The wordproc is currently receiving keypresses.
    // if the dialog is active, is should forward the keypress to it.

    private:
    SDL_Renderer * renderer = NULL;
    Font * dialog_font = NULL;
    void WriteCentered(std::wstring text, int posy);
};



#endif