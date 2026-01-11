#include "audio.h"

static const char * soundFiles[] = {
    "resources/sounds/pluck_2.ogg",
    "resources/sounds/woosh.ogg"
};

static const char *  musicFiles[] = {
    "resources/music/dreamscape/oneheart-snowfall.ogg",
    "resources/music/dreamscape/oneheart-apathy.ogg",
    "resources/music/dreamscape/danielmp3-greentoblue.ogg"
};

audio::audio(){ // constructor
    std::cout << "Loading all music/SFX\n";
    for(size_t i = 0; i < music_ta.size(); i++){
        music_ta[i] = Mix_LoadMUS(musicFiles[i]);
        if(music_ta[i] == NULL){
            std::cout << "Music could not be loaded (" << Mix_GetError() << ").\n";
        }
    }

    for(size_t i = 0; i < sound_ta.size(); i++){
        sound_ta[i] = Mix_LoadWAV(soundFiles[i]);
        if(sound_ta[i] == NULL){
            std::cout << "Sound could not be loaded (" << Mix_GetError() << ").\n";
        }
    }
}

audio::~audio(){
    std::cout << "Destroying all music/SFX" << std::endl;
    for(auto& m : music_ta){
        if(m){
            Mix_FreeMusic(m);
        }
    }

    for(auto& s : sound_ta){
        if(s){
            Mix_FreeChunk(s);
        }
    }
}

void audio::PlaySound(SoundID id){
    Mix_PlayChannel(-1, sound_ta[static_cast<size_t>(id)], 0);
}

void audio::PlayMusic(MusicID id){
    std::cout << "Playing music \n";
    Mix_PlayMusic(music_ta[static_cast<size_t>(id)], -1);
}