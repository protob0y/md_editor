#include "menu.h"

menu::menu(SDL_Renderer * rinstance, Font * finstance, TextureManager * tminstance){
    renderer = rinstance;
    texMan = tminstance;
    menu_font = finstance;

    splash = texMan->LoadTexture("resources/splash.png");
    //menu_font->setColor(13, 13, 150);

    //Music
    music = Mix_LoadMUS("resources/music1.ogg");
    if(music == NULL){
        std::cout << "Error reading music file." << std::endl;
    }
    sound_effect = Mix_LoadWAV("resources/pluck.wav");
    if(sound_effect == NULL){
        std::cout << "Error reading music file." << std::endl;
    }
    if(Mix_PlayingMusic() == 0){
        Mix_PlayMusic(music, -1);
    }
}

menu::~menu(){
    if(music != NULL){
        Mix_FreeMusic(music);
    }
    if(sound_effect != NULL){
        Mix_FreeChunk(sound_effect);
    }
}

void menu::RenderMenu(){
    texMan->RenderTexture(splash);

    switch(mState){
        case MAIN:{
        WriteCentered(L"Welcome to main menu!", 150);
        break;
        
        case LOAD:
        WriteCentered(L"Select a file!", 150);
        break;
        
        case EXIT:
        WriteCentered(L"Are you sure?", 150);
        break;

        default:
        break;
        }
    }

    //render menu items
    const int y_start = 200;
    const int y_spacing = 40;
    for(int i = 0; i < menuItems[mState].size(); i++){
        std::wstring item = menuItems[mState][i];
        if(i == cursorpos){
            item.insert(0, L"-> ");
            item.append(L" <-");
        }
        WriteCentered(item, y_start + i * y_spacing);
    }

}

state menu::registerKeypress(SDL_Event * event){
    switch(event->key.keysym.sym){
        case SDLK_RETURN:
        switch(mState){
            case MAIN:
            switch(cursorpos){
                case 0:
                mState = START;
                break;
                case 1:
                mState = SETTINGS;
                break;
                case 2:
                mState = EXIT;
                break;
            }
            break;

            case START:
            switch (cursorpos){
                case 0:
                Mix_FadeOutMusic(1000);
                return EDITOR;
                break;
                case 1:
                mState = LOAD;
                break;
                case 2:
                mState = MAIN;
                break;
            }
            break;

            case LOAD:
            std::cout << "Not implemented yet." << std::endl;
            break;

            case SETTINGS:
            std::cout << "Stub" << std::endl;
            break;

            case EXIT:
            switch(cursorpos){
                case 0:
                std::cout << "Exiting not implemented yet. just close the program n00b" << std::endl;
                case 1:
                mState = MAIN;
            }


        }
        cursorpos = 0;
        break;

        case SDLK_UP:
        Mix_PlayChannel(-1, sound_effect, 0);
        if(cursorpos > 0){
            cursorpos--;
        }
        break;

        case SDLK_DOWN:
        Mix_PlayChannel(-1, sound_effect, 0);
        if(cursorpos < (menuItems[mState].size() - 1)){
            cursorpos++;
        }
        break;
    }
    return MENU;
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