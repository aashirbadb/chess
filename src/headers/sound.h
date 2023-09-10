#pragma once
#include <unordered_map>
#include <iostream>
#include <SDL2/SDL_mixer.h>

enum class Sound
{
    Capture = 0,
    Castle,
    Check,
    Move,
    Promote,
    Victory,
    Draw,
    Defeat,
    ButtonClick,
    ___LAST // used to get total number of sounds
};
const int TotalSounds = (int)Sound::___LAST;

// returns the (relative) path of the sound asset
const char *getSoundPath(Sound _s);

class SoundManager
{
    static std::unordered_map<Sound, Mix_Chunk *> SOUNDS;

public:
    SoundManager();
    ~SoundManager();

    void init();
    void play(Sound _sound);
};