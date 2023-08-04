#include "headers/game.h"
#include <iostream>
#include <SDL2/SDL_blendmode.h>

Game::Game()
{
    quit = false;
    render_requested = true;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SQUARE_SIZE * 8, SQUARE_SIZE * 8, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    font = TTF_OpenFont("assets/Roboto.ttf", 24);

    board = new Board(SOME_FEN);
    selected_piece = nullptr;
}

Game::~Game()
{
    delete board;

    TTF_CloseFont(font);
    TTF_Quit();

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

        if (render_requested)
            drawBoard();
        render_requested = false;
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
    case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            handleMouseLeftClick();
            break;

        default:
            break;
        }
        break;
    }
}

void Game::drawBoard()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

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

    // Highlight selected piece and show valid moves
    if (selected_piece != nullptr)
    {
        Coordinate pos = selected_piece->getPosition();
        SDL_Rect rect = {pos.x * SQUARE_SIZE, pos.y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
        SDL_RenderFillRect(renderer, &rect);

        std::vector<Move> moves = selected_piece->getLegalMoves(*board);

        for (int i = 0; i < moves.size(); i++)
        {
            SDL_Rect rect = {moves[i].end.x * SQUARE_SIZE, moves[i].end.y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 64);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Draw pieces
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            SDL_Rect rect;
            rect.x = x * SQUARE_SIZE;
            rect.y = y * SQUARE_SIZE;
            rect.w = SQUARE_SIZE;
            rect.h = SQUARE_SIZE;

            if (board->getPieceAt({x, y}) != nullptr)
            {
                // TODO: add piece textures

                // Causes memory leak

                std::string symbol;
                symbol.push_back(board->getPieceAt({x, y})->getSymbol());
                SDL_Surface *text = TTF_RenderText_Solid(font, symbol.c_str(), {255, 0, 0});
                SDL_Texture *texttexture = SDL_CreateTextureFromSurface(renderer, text);
                SDL_Rect dest = {x, y, text->w, text->h};
                SDL_RenderCopy(renderer, texttexture, NULL, &rect);
                SDL_DestroyTexture(texttexture);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

void Game::handleMouseLeftClick()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    x /= SQUARE_SIZE;
    y /= SQUARE_SIZE;
    Coordinate selected_coord{x, y};
    Piece *piece = board->getPieceAt(selected_coord);

    if (selected_piece != nullptr)
    {
        std::vector<Move> moves = selected_piece->getLegalMoves(*board);

        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i].end == selected_coord)
            {
                std::cout << moves[i] << std::endl;
                board->performMove(moves[i]);
                selected_piece = nullptr;
                render_requested = true;
                return;
            }
        }
    }
    if (piece != nullptr && piece == selected_piece)
    {
        selected_piece = nullptr;
    }
    else if (piece != nullptr && board->getIsWhiteTurn() == piece->isWhite())
    {
        selected_piece = piece;
    }
    else
    {
        selected_piece = nullptr;
    }

    render_requested = true;
}