#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
#include "wordproc.h"
#include "font.h"
#include "state.h"
#include "menu.h"
#include "texturemanager.h"
#include "dialog.h"

//Feature request:
// implement saving
// automatic line break
// main menu
// --> implement loading files
// auto changing wallpaper after a random amount of time which can be accelerated by typing

//bugfixes:

using namespace std;

void shutdown(SDL_Window * window, SDL_Renderer * renderer);

int main(){
    state current_state = MENU; // state is defined in state.h

    char backgroundImagePath[] = "resources/sunset.jpg";

    cout << "Welcome." << endl;

    // ------------
    // SDL AND WINDOW INIT
    // ------------
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        return -1;
    }
    cout << "SDL Inited." << endl;

    // init JPG and PNG
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initResult = IMG_Init(imgFlags);
    if ((initResult & imgFlags) != imgFlags) {
        cout << "Error initializing SDL_image: " << IMG_GetError() << endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    // init Audio
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        cout << "Error initialiting SDL_mixer: " << Mix_GetError() << endl;
        IMG_Quit();
        SDL_Quit();
        return -1;
    }

    window = SDL_CreateWindow("Retrowriter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 480, SDL_WINDOW_SHOWN); // | SDL_WINDOW_ALLOWHIGHDPI
    if(!window){
        cout << "Error creating window: " << SDL_GetError() << endl;
        shutdown(NULL, NULL);
        return -1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Error creating renderer: " << SDL_GetError() << endl;
        shutdown(window, NULL);
        return -1;
    }

    // instantiate fonts and wordprocessor
    TextureManager texManager(renderer);
    Font small_font(renderer, "resources/PressStart.ttf", 18);
    Font menu_font(renderer, "resources/PressStart.ttf", 25);
    Font dialog_font(renderer, "resources/PressStart.ttf", 14);
    menu main_menu(renderer, &menu_font, &texManager);
    Dialog saveDialog(renderer, &dialog_font);
    WordProc wordprocessor(renderer, &small_font, &texManager, &saveDialog);

    // ----------
    // MAIN LOOP
    // ----------

    bool running = true;
    SDL_Event event;
    SDL_StartTextInput();

    //Timing
    const int FPS = 60;
    const int frameDelay = 1000 / FPS; // how long we want to keep one frame on screen
    Uint32 frameStart;
    int frameTime; // how long it took to draw this frame

    while(running){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if(event.type == SDL_TEXTINPUT){
                wordprocessor.RegisterTextinput(event.text.text);
            }
            else if(event.type == SDL_KEYDOWN){
                switch(current_state){
                    case MENU:
                    current_state = main_menu.registerKeypress(&event);
                    break;
                    
                    case EDITOR:
                    wordprocessor.RegisterKeypress(&event);
                }
            }
        }
        frameStart = SDL_GetTicks();

        switch(current_state){
            case MENU:
            main_menu.RenderMenu();
            break;

            case EDITOR:
            wordprocessor.RenderScreen();
            small_font.renderTable(renderer, 200, 400);
        }

        
        SDL_RenderPresent(renderer); // Render to screen.

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay){
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_StopTextInput();

    shutdown(window, renderer);

    return 0;
}

void shutdown(SDL_Window * window, SDL_Renderer * renderer){
    if(renderer){
        SDL_DestroyRenderer(renderer);
    }
    if(window){
        SDL_DestroyWindow(window);
    }
    IMG_Quit();
    SDL_Quit();

    cout << "Goodbye" << endl;
}
