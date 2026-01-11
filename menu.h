#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "font.h"
#include "texturemanager.h"
#include "audio.h"
#include <string>
#include <map>
#include <vector>

enum class AppCommand{ // command to be sent back to main
    None,
    StartGame,
    ToggleMusic,
    ToggleSound,
    ToggleFullscreen,
    VolumeUp,
    VolumeDown,
    LoadGame,
    QuitGame
};

struct MenuLayer; // defined later

struct MenuItem{
    std::wstring label;
    AppCommand cmd = AppCommand::None;
    MenuLayer * child = nullptr; // points to sub menu
};

struct MenuLayer{
    std::wstring title; // title of this menu
    std::vector<MenuItem> entries;
};

class menu{
    public:
    menu(audio * ainstance, SDL_Renderer * rinstance, Font * finstance, TextureManager * tminstance);
    ~menu();
    void RenderMenu();
    void registerKeypress(SDL_Event * event);
    AppCommand getPendingCommand();
    
    private:
    SDL_Renderer * renderer = NULL;
    Font * menu_font = NULL;
    TextureManager * texMan = NULL;
    audio * Audio = NULL;

    SDL_Texture * splash = NULL;

    AppCommand pendingCmd = AppCommand::None;

    MenuLayer rootMenu;
    MenuLayer settingsMenu;
    MenuLayer exitMenu;

    std::vector<MenuLayer*> menuStack; // last element indicates current sub menu

    int cursorpos = 0;
    
    void buildMenu();
    void buildLoadGameMenu();

    void WriteCentered(std::wstring text, int posy);
};


#endif