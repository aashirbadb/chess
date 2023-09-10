#include "headers/sound.h"
#include "headers/game.h"

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
    case Sound::Defeat:
        return "assets/defeat.mp3";
    case Sound::ButtonClick:
        return "assets/buttonpress.wav";
    default:
        throw;
    }
}

SoundManager::SoundManager()
{
}

void SoundManager::init()
{
    for (int i = 0; i < TotalSounds; i++)
    {
        Sound snd = (Sound)i;
        const char *path = getSoundPath(snd);
        std::string abspath = Game::BASE_PATH + "../" + getSoundPath(snd);
        Mix_Chunk *chunk = Mix_LoadWAV(abspath.c_str());
        if (chunk == nullptr)
        {
            std::cerr << "Error loading " << path << ": " << Mix_GetError() << std::endl;
        }
        else
        {
            SOUNDS[snd] = chunk;
        }
    }
}

SoundManager::~SoundManager()
{
    for (auto &it : SOUNDS)
    {
        Mix_FreeChunk(it.second);
    }
}

void SoundManager::play(Sound _s)
{
    Mix_Chunk *sound = SOUNDS[_s];

    if (sound)
    {
        Mix_PlayChannel(-1, sound, 0);
    }
}