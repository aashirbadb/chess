#pragma once
#include <SDL2/SDL_mixer.h>
#include <unordered_map>

enum class Sound
{
    Capture = 0,
    Castle,
    Check,
    Move,
    Promote,
    Victory,
    Draw,
    ___LAST // used to get total number of sounds
};
const int TotalSounds = (int)Sound::___LAST;

const char *getSoundPath(Sound _s);
Sound getSoundFromInt(int i);

class SoundManager
{
    Mix_Chunk *capture, *castle, *check, *move, *promote;
    static std::unordered_map<Sound, Mix_Chunk *> SOUNDS;

public:
    SoundManager();
    ~SoundManager();
    void playCaptureSound() const;
    void playCastleSound() const;
    void playCheckSound() const;
    void playMoveSound() const;
    void playPromoteSound() const;
    void play(Sound _sound);
};