// [SDL_mixer] can load FLAC, MP3, Ogg, VOC, and WAV format audio
// (https://wiki.libsdl.org/SDL2_mixer/FrontPage)
// Chunks live in RAM (sound effects), multiple can play at once or together with music
// Music is being streamed from storage, only one can play at once

#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/* Music/SFX names, storage location and instanced need to be stored alongside
each other. We decide on the approach storing the names to access Music/SFX
in enum class with ascending ID. Mix_Chunk* types are stored in std::array.
Make sure to adjust filename list in audio.cpp and array at the same time.
Accessing can be done by audio.playSound(SoundID::Pluck). This approach ensures
potential failure happening during compile time, not during runtime.
Asset definition in .h (not super clean) enables main to use musicID
*/

enum class SoundID : size_t{
    Pluck = 0,
    Woosh,
    COUNT
};

enum class MusicID : size_t{
    snowfall = 0,
    apathy,
    greentoblue,
    COUNT
};

class audio{
    public:
    audio();
    ~audio();
    void PlaySound(SoundID id);
    void PlayMusic(MusicID id);

    private:
    std::array<Mix_Chunk*, (size_t)SoundID::COUNT> sound_ta{}; // array with COUNT entries
    std::array<Mix_Music*, (size_t)MusicID::COUNT> music_ta{}; // {} inits with nullptrs

};

#endif