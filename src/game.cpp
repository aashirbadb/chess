#include <iostream>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_mixer.h>
#include "headers/game.h"
#include "headers/gameMenu.h"

Game::Game()
{
    quit = false;
    render_requested = true;
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    // window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetSurfaceBlendMode(SDL_GetWindowSurface(window), SDL_BLENDMODE_BLEND);

    SDL_SetWindowMinimumSize(window, 320, 180);

    sizes = {0, 0, 0, 0, 0, 0, 0, 0};

    calculateWindowSize();

    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    sound = new SoundManager();

    scenes.push(new GameMenu(this));
}

Game::~Game()
{
    TTF_Quit();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    delete sound;

    // clear the scenes stack
    while (!scenes.empty())
    {
        scenes.top()->~GameScene();
        scenes.pop();
    }
}

void Game::start()
{
    while (!quit)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;

        case SDL_WINDOWEVENT:
            calculateWindowSize();
            requestRender();
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            scenes.top()->handleEvent(event);
            break;

        default:
            requestRender();
            break;
        }
        // No need to render if user has quit
        if (quit)
            break;

        // Render board/game only if render is requested
        if (render_requested)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            scenes.top()->render();
            SDL_RenderPresent(renderer);
        }
        render_requested = false;
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

void Game::requestRender()
{
    render_requested = true;
}

void Game::pushScene(GameScene *sc)
{
    scenes.push(sc);
    render_requested = true;
}

GameScene *Game::popScene()
{
    GameScene *temp = scenes.top();
    scenes.pop();
    render_requested = true;
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
        sound->play(_sound);
}