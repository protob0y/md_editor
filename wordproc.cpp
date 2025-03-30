#include "wordproc.h"

WordProc::WordProc(SDL_Renderer * rinstance, Font * finstance, TextureManager * tminstance, Dialog * dinstance){
    smallfont = finstance;
    renderer = rinstance;
    texMan = tminstance;
    saveDialog = dinstance;

    background = texMan->LoadTexture("resources/snow.png");

    std::wstring line = L"Write here...";
    cursor_col = line.size();
    document.push_back(line);

    cursorBlinkStart = SDL_GetTicks();
}

void WordProc::RenderScreen(){
    texMan->RenderTexture(background);

    int posx = margin_left;
    int posy = margin_top;
    for(int line = 0; line < document.size(); line++){
        for(Uint16 c : document[line]){
            smallfont->renderChar(renderer, c, posx, posy);
            posx += smallfont->getGlyphWidth();
        }
        posx = margin_left;
        posy += (smallfont->getGlyphHeight() + addLineSpacing);
    }

    if(!showSaveDialog){
        DrawCursor();
    }
    
    if(showSaveDialog){
        saveDialog->RenderDialog();
    }
}

void WordProc::RegisterTextinput(const char * utf8Text){
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wletter = converter.from_bytes(utf8Text);
    document[cursor_line].insert(cursor_col, wletter);
    cursor_col++;

    cursorBlinkShowing = true;
}

void WordProc::RegisterKeypress(SDL_Event * event){
    switch(event->key.keysym.sym){
        case SDLK_RETURN: {
            if(cursor_col != document[cursor_line].size()){ // if cursor not at EOL
                std::wstring right = document[cursor_line].substr(cursor_col);  // cut right part and move to next line
                document[cursor_line] = document[cursor_line].substr(0, cursor_col);
                document.insert(document.begin() + cursor_line + 1, right);
                cursor_line++;
                cursor_col = 0;
                break;
            }

            //this only happens if cursor was at EOL
            if((cursor_line + 1) == document.size()){ // if cursor in last line of document
                document.push_back(L"");
                cursor_line++;
                cursor_col = 0;
            }
            else{
                document.insert(document.begin() + cursor_line + 1, L"");
                cursor_line++;
                cursor_col = 0;
            }
            
            break;
        }
        case SDLK_BACKSPACE: {
            if(cursor_col == 0)
            {
                if(cursor_line != 0){
                    std::wstring buffer = document[cursor_line];
                    document.erase(document.begin() + cursor_line);
                    cursor_line--;
                    cursor_col = document[cursor_line].size();
                    document[cursor_line].append(buffer);
                }
            }
            else{
                cursor_col--;
                document[cursor_line].erase(cursor_col, 1);
            }
            break;
        }
        case SDLK_LEFT: {
            if(cursor_col == 0){
                if(cursor_line != 0)
                {
                  cursor_line--;
                  cursor_col = document[cursor_line].size();
                }
            }
            else{
                cursor_col--;
            }
            break;
        }
        case SDLK_RIGHT: {
            if(cursor_col < document[cursor_line].size()){
                cursor_col++;
            }
            else if((cursor_line + 1) < document.size()){
                cursor_col = 0;
                cursor_line++;
            }
            break;
        }
        case SDLK_UP: {
            if(cursor_line != 0){
                cursor_line--;
                if(cursor_col > document[cursor_line].size()){
                    cursor_col = document[cursor_line].size();
                }
            }
            break;
        }
        case SDLK_DOWN: {
            if((cursor_line + 1) < document.size())
            {
                cursor_line++;
                if(cursor_col > document[cursor_line].size()){
                    cursor_col = document[cursor_line].size();
                }
            }
            break;
        }
        case SDLK_ESCAPE:
            showSaveDialog = true;
            break;
    }
    
    cursorBlinkShowing = true;
}

void WordProc::DrawCursor(){
    int glyphWidth = smallfont->getGlyphWidth();
    int glyphHeight = smallfont->getGlyphHeight();
    const int cursorAddY = 2;

    SDL_Rect cursorRect = {
        margin_left + cursor_col * glyphWidth,
        margin_top + cursor_line * (glyphHeight + addLineSpacing) - cursorAddY,
        3,
        glyphHeight + cursorAddY
    };

    
    if(SDL_GetTicks() > (cursorBlinkStart + cursorBlinkInterval)){
        cursorBlinkStart = SDL_GetTicks();
        cursorBlinkShowing = !cursorBlinkShowing;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if(cursorBlinkShowing){
        SDL_RenderFillRect(renderer, &cursorRect);
    }

}