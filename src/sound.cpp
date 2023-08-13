#include "headers/sound.h"

std::unordered_map<Sound, Mix_Chunk *> SoundManager::SOUNDS;

const char *getSoundPath(Sound _s)
{
    switch (_s)
    {
    case Sound::Capture:
        return "assets/capture.mp3";
    case Sound::Castle:
        return "assets/castle.mp3";
    case Sound::Check:
        return "assets/check.mp3";
    case Sound::Move:
        return "assets/move.mp3";
    case Sound::Promote:
        return "assets/promote.mp3";
    case Sound::Victory:
        return "assets/victory.mp3";
    case Sound::Draw:
        return "assets/draw.mp3";
    default:
        throw;
    }
}

Sound getSoundFromInt(int i)
{
    switch (i)
    {
    case 0:
        return Sound::Capture;
    case 1:
        return Sound::Castle;
    case 2:
        return Sound::Check;
    case 3:
        return Sound::Move;
    case 4:
        return Sound::Promote;
    case 5:
        return Sound::Victory;
    case 6:
        return Sound::Draw;

    default:
        throw 2;
        break;
    }
}

SoundManager::SoundManager()
{
    capture = Mix_LoadWAV("assets/capture.wav");
    castle = Mix_LoadWAV("assets/castle.wav");
    check = Mix_LoadWAV("assets/check.wav");
    move = Mix_LoadWAV("assets/move.wav");
    promote = Mix_LoadWAV("assets/promote.wav");

    for (int i = 0; i < TotalSounds; i++)
    {
        Sound snd = getSoundFromInt(i);
        SOUNDS[snd] = Mix_LoadWAV(getSoundPath(snd));
    }

    SOUNDS[Sound::Capture] = Mix_LoadWAV("assets/capture.mp3");
}

SoundManager::~SoundManager()
{
    Mix_FreeChunk(capture);
    Mix_FreeChunk(castle);
    Mix_FreeChunk(check);
    Mix_FreeChunk(move);
    Mix_FreeChunk(promote);
}

void SoundManager::playCaptureSound() const
{
    Mix_PlayChannel(-1, capture, 0);
}

void SoundManager::playCastleSound() const
{
    Mix_PlayChannel(-1, castle, 0);
}

void SoundManager::playCheckSound() const
{
    Mix_PlayChannel(-1, check, 0);
}

void SoundManager::playMoveSound() const
{
    Mix_PlayChannel(-1, move, 0);
}

void SoundManager::playPromoteSound() const
{
    Mix_PlayChannel(-1, promote, 0);
}

void SoundManager::play(Sound _s)
{
    Mix_Chunk *sound = SOUNDS[_s];

    if (sound)
    {
        Mix_PlayChannel(-1, sound, 0);
    }
}