//this class is instantiated by wordproc and gets called when the user presses the esc key

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "font.h"
#include <string>
#include <locale>
#include <codecvt>

#ifndef DIALOG_H
#define DIALOG_H

enum DialogStatus{
    OPEN,
    SAVE,
    ABORT
};

class Dialog{
    public:
    Dialog(SDL_Renderer * rinstance, Font * finstance);
    void RenderDialog();
    void RegisterKeypress(SDL_Event * event);
    void RegisterTextInput(std::wstring letter);
    void setDialogStatus(DialogStatus ds);
    DialogStatus getDialogStatus();
    std::string getFilePath();
    DialogStatus ds = OPEN;

    private:
    SDL_Renderer * renderer = NULL;
    Font * dialog_font = NULL;
    void WriteCentered(std::wstring text, int posy);
    std::wstring filename = L"File_1";
};



#endif