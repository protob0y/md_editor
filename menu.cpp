#include "menu.h"

menu::menu(audio * ainstance, SDL_Renderer * rinstance, Font * finstance, TextureManager * tminstance){
    Audio = ainstance;
    renderer = rinstance;
    texMan = tminstance;
    menu_font = finstance;

    splash = texMan->LoadTexture("resources/splash.png");
    //menu_font->setColor(13, 13, 150);

    buildMenu(); // populate vector<MenuItem> rootMenu
}

menu::~menu(){

}

void menu::RenderMenu(){
    texMan->RenderTexture(splash); // render BG

    MenuLayer * activeMenu = menuStack.back();
    //render menu
    int text_y_pos = 200;
    const int y_spacing = 40;
    WriteCentered(activeMenu->title, text_y_pos); // write title
    text_y_pos += y_spacing;
    for(int i = 0; i < activeMenu->entries.size(); i++){
        std::wstring item = activeMenu->entries[i].label;
        if(i == cursorpos){
            item.insert(0, L"-> ");
            item.append(L" <-");
        }
        WriteCentered(item, text_y_pos); // write entry
        text_y_pos += y_spacing;
    }
    if(menuStack.size() > 1){
        WriteCentered(L"ESC to go back", text_y_pos);
    }
}

void menu::registerKeypress(SDL_Event * event){
    MenuLayer * activeMenu = menuStack.back(); // get last element of stack
    switch(event->key.keysym.sym){
        case SDLK_RETURN:{
        MenuLayer * child = activeMenu->entries[cursorpos].child;
        if(child != nullptr){ // there is a ptr to a menuLayer in this entry
            menuStack.push_back(child);
            std::cout << "Enter sub menu " << child << std::endl;
            cursorpos = 0;
        }
        else{ // no ptr to sub menu layer
            std::cout << "send AppCmd" << std::endl;
            pendingCmd = activeMenu->entries[cursorpos].cmd;
        }
        break;}

        case SDLK_UP:
        Audio->PlaySound(SoundID::Pluck);
        if(cursorpos > 0){
            cursorpos--;
        }
        break;

        case SDLK_DOWN:
        Audio->PlaySound(SoundID::Pluck);
        if(cursorpos < (activeMenu->entries.size() - 1)){
            cursorpos++;
        }
        break;

        case SDLK_ESCAPE:
        Audio->PlaySound(SoundID::Woosh);
        if(menuStack.size() > 1){ // if sub / sub sub menu is active
            menuStack.pop_back(); // go menu layer up
            cursorpos = 0;
        }
        break;
    }
}

void menu::buildMenu(){
    // root menu
    rootMenu.title = L"Welcome to main menu!";
    rootMenu.entries.clear();
    MenuItem newGame{L"New Game", AppCommand::StartGame, {}};
    MenuItem loadGame{L"Load Game", AppCommand::None, {}};
    MenuItem settings{L"Settings", AppCommand::None, &settingsMenu};
    MenuItem exit{L"Exit", AppCommand::QuitGame, &exitMenu};
    rootMenu.entries = {newGame, loadGame, settings, exit};

    // settings menu
    settingsMenu.title = L"Game Settings";
    settingsMenu.entries.clear();
    MenuItem setTglMusic{L"Toggle Music", AppCommand::ToggleMusic, {}};
    MenuItem setTglSound{L"Toggle Sound", AppCommand::ToggleSound, {}};
    MenuItem setTglFullscr{L"Toggle Fullscreen", AppCommand::ToggleFullscreen, {}};
    settingsMenu.entries = {setTglMusic, setTglSound, setTglFullscr};

    // exit are-you-sure? menu
    exitMenu.title = L"Are you sure to exit?";
    exitMenu.entries.clear();
    MenuItem exitYes{L"Yes, exit", AppCommand::QuitGame, {}};
    exitMenu.entries = {exitYes};

    menuStack.clear();
    menuStack.push_back(&rootMenu); // start with main menu layer on stack
}

void menu::buildLoadGameMenu(){

}

AppCommand menu::getPendingCommand(){
    return pendingCmd;
    pendingCmd = AppCommand::None;
}

void menu::WriteCentered(std::wstring text, int posy){
    int glyphWidth = menu_font->getGlyphWidth();
    int textWidth = text.length() * glyphWidth;
    int posx = 360 - (textWidth / 2);

    for(Uint16 c : text){
        menu_font->renderChar(renderer, c, posx, posy);
        posx += glyphWidth;
    }
}