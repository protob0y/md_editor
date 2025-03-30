#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "state.h"
#include "font.h"
#include "texturemanager.h"
#include <string>
#include <map>
#include <vector>

enum menu_state{MAIN, START, LOAD, SETTINGS, EXIT};

class menu{
    public:
    menu(SDL_Renderer * rinstance, Font * finstance, TextureManager * tminstance);
    ~menu();
    void RenderMenu();
    state registerKeypress(SDL_Event * event);
    
    private:
    Mix_Music * music = NULL;
    Mix_Chunk * sound_effect = NULL;
    SDL_Renderer * renderer = NULL;
    Font * menu_font = NULL;
    TextureManager * texMan = NULL;

    SDL_Texture * splash = NULL;
    std::map<menu_state, std::vector<std::wstring>> menuItems = {
        {MAIN, {L"Play", L"Settings", L"Exit"}},
        {START, {L"New Game", L"Load", L"Back"}},
        {LOAD, {L"FILE#1", L"Back"}},
        {EXIT, {L"Yes", L"No"}},
        {SETTINGS, {L"Fullscreen", L"Back"}}
    };
    menu_state mState = MAIN;
    int cursorpos = 0;

    void WriteCentered(std::wstring text, int posy);
};


#endif