#include <iostream>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_mixer.h>
#include "headers/game.h"
#include "headers/gameMenu.h"

Game::Game()
{
    quit = false;

    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    // window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetWindowMinimumSize(window, 320, 180);

    sizes = {0, 0, 0, 0, 0, 0, 0, 0};

    calculateWindowSize();

    TTF_Init();

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    soundManager.init();

    muted = false;
    scenes.push(new GameMenu(this));
}

Game::~Game()
{
    TTF_Quit();
    Mix_Quit();
    // clear the scenes stack
    while (!scenes.empty())
    {
        scenes.top()->~GameScene();
        scenes.pop();
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::start()
{
    Uint64 timeperframe = 1000 / FPS;
    while (!quit)
    {
        Uint64 start = SDL_GetTicks64();

        SDL_PollEvent(&event);

        // SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;

        case SDL_WINDOWEVENT:
            calculateWindowSize();
            scenes.top()->handleResize();
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            scenes.top()->handleEvent(event);
            break;

        default:
            break;
        }

        // No need to render if user has quit
        if (quit)
            break;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        scenes.top()->render();
        SDL_RenderPresent(renderer);

        scenes.top()->update();
        Uint64 end = SDL_GetTicks64();

        float elapsedMS = (end - start);

        // Cap to 60 FPS
        SDL_Delay(std::max((long int)(1000 / FPS - elapsedMS), (long int)0));
    }
}

SDL_Renderer *Game::getRenderer()
{
    return renderer;
}

void Game::quitGame()
{
    quit = true;
}

void Game::pushScene(GameScene *sc)
{
    scenes.push(sc);
}

GameScene *Game::popScene()
{
    GameScene *temp = scenes.top();
    scenes.pop();

    return temp;
}

GameScene *Game::currentScene()
{
    return scenes.top();
}

void Game::calculateWindowSize()
{
    int h, w;
    SDL_GetWindowSize(window, &w, &h);

    int tsize = std::min(h, w) / 8;
    int bsize = tsize * 8;
    int topbottomoffset = std::max((h - w) / 2, (h - bsize) / 2);
    int leftrightoffset = std::max((w - h) / 2, (w - bsize) / 2);

    WindowSize newSize = {
        .height = h,
        .width = w,
        .topOffset = topbottomoffset,
        .bottomOffset = topbottomoffset,
        .leftOffset = leftrightoffset,
        .rightOffset = leftrightoffset,
        .tileSize = tsize,
        .boardSize = tsize * 8,
    };

    if (newSize != sizes)
    {
        sizes = newSize;
    }
}

WindowSize Game::getWindowSize() const
{
    return sizes;
}

void Game::playSound(Sound _sound)
{
    if (!muted)
        soundManager.play(_sound);
}

void Game::setMuted(bool mt)
{
    muted = mt;
}

bool Game::getMuted()
{
    return muted;
}