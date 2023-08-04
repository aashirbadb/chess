#include "headers/game.h"

Game::Game()
{
    quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SQUARE_SIZE * 8, SQUARE_SIZE * 8, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    board = new Board(STARTING_FEN);
}

Game::~Game()
{
    delete board;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::start()
{
    while (!quit)
    {
        handleEvent();
        if (quit)
            break;

        drawBoard();

        SDL_RenderPresent(renderer);
    }
}

void Game::handleEvent()
{
    SDL_WaitEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        quit = true;
        break;
    }
}

void Game::drawBoard()
{
    // Draw squares
    int startPos = 0;
    for (int y = 0; y < 8; y++)
    {
        for (int x = startPos; x < 8; x += 2)
        {
            SDL_Rect rect;
            rect.x = x * SQUARE_SIZE;
            rect.y = y * SQUARE_SIZE;
            rect.w = SQUARE_SIZE;
            rect.h = SQUARE_SIZE;
            SDL_SetRenderDrawColor(renderer, 238, 238, 210, 200);
            SDL_RenderFillRect(renderer, &rect);
        }
        startPos = 1 - startPos;
    }

    // Draw pieces
    // TODO:
}