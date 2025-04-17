#include "dialog.h"


Dialog::Dialog(SDL_Renderer * rinstance, Font * finstance){
    renderer = rinstance;
    dialog_font = finstance;
}

void Dialog::RenderDialog(){
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    const int box_size_x = 50; // expressed in percent of screen size
    const int box_size_y = 20;

    // Draw greyed out background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 60);
    SDL_Rect overlay = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Draw snowflakes
    // ...


    // Draw Message box
    const int outline = 5;
    const int box_w = box_size_x * width * 0.01;
    const int box_h = box_size_y * height * 0.01;
    const int box_x = (width / 2) - (box_w / 2);
    const int box_y = (height / 2) - (box_h / 2);
    SDL_Rect outline_box  = {box_x - outline, box_y - outline, box_w + 2 * outline, box_h + 2 * outline};
    SDL_Rect box = {box_x, box_y, box_w, box_h};

    SDL_SetRenderDrawColor(renderer, 99, 169, 226, 255);
    SDL_RenderFillRect(renderer, &outline_box);
    SDL_SetRenderDrawColor(renderer, 22, 122, 204, 255);
    SDL_RenderFillRect(renderer, &box);


    //Draw Text
    WriteCentered(L"Enter filename to save:", 200);
    WriteCentered(filename, 230);
    WriteCentered(L"Or press ESC to return", 260);
    
}


void Dialog::WriteCentered(std::wstring text, int posy){
    int glyphWidth = dialog_font->getGlyphWidth();
    int textWidth = text.length() * glyphWidth;
    int posx = 360 - (textWidth / 2);

    for(Uint16 c : text){
        dialog_font->renderChar(renderer, c, posx, posy);
        posx += glyphWidth;
    }
}

void Dialog::RegisterKeypress(SDL_Event * event){
    switch(event->key.keysym.sym){
        case SDLK_ESCAPE:
        ds = ABORT;
        break;
        case SDLK_RETURN:
        if(filename.size() > 0){
            ds = SAVE;
        }
        break;
        case SDLK_BACKSPACE:
        if(filename.size() > 0){
            filename.erase(filename.size() - 1);
        }
        break;
    }
}

void Dialog::RegisterTextInput(std::wstring letter){
    filename.append(letter);
}

std::string Dialog::getFilePath(){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; // convert to std::string
    return converter.to_bytes(filename);
}