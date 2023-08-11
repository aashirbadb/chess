#include <iostream>
#include "headers/game.h"
#include "headers/ChessGameScene.h"

Game::Game()
{
    quit = false;
    render_requested = true;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SQUARE_SIZE * 8, SQUARE_SIZE * 8, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    scenes.push(new ChessGame(this));
}

Game::~Game()
{

    TTF_Quit();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

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

        default:
            scenes.top()->handleEvent(event);

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