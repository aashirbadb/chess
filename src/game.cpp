#include "headers/game.h"
#include "headers/texture.h"
#include <iostream>
#include <SDL2/SDL_blendmode.h>

char Game::columnlabel[9] = "abcdefgh";
char Game::rowlabel[9] = "12345678";

namespace color
{
    const SDL_Color WHITE = {255, 255, 255, SDL_ALPHA_OPAQUE};
    const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
    const SDL_Color RED = {255, 0, 0, SDL_ALPHA_OPAQUE};
    const SDL_Color GREEN = {0, 255, 0, SDL_ALPHA_OPAQUE};
    const SDL_Color BLUE = {0, 0, 255, SDL_ALPHA_OPAQUE};

    const SDL_Color WHITE_TILE = WHITE;
    const SDL_Color BLACK_TILE = BLACK;
    const SDL_Color CHECK_TILE = RED;
    const SDL_Color SELECTED_TILE = GREEN;
    const SDL_Color MOVE_TILE = GREEN;
}

Game::Game()
{
    quit = false;
    render_requested = true;
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SQUARE_SIZE * 8, SQUARE_SIZE * 8, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    board = new Board(STARTING_FEN);
    selected_piece = nullptr;
}

Game::~Game()
{
    delete board;

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
        // No need to render if user has quit
        if (quit)
            break;

        // Render board/game only if render is requested
        if (render_requested)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            drawBoard();
            SDL_RenderPresent(renderer);
        }
        render_requested = false;

        GameState state = board->getGameState();
        // TODO: Currently game just stops if the game ends
        quit = (state == GameState::WhiteWins) || (state == GameState::BlackWins) || (state == GameState::Stalemate) || (state == GameState::Draw);
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
    // Draw board tiles
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            SDL_Color tilecolor;
            if (board->getBoardColorAt(x, y))
            {
                tilecolor = color::WHITE;
            }
            else
            {
                tilecolor = color::BLACK;
            }

            SDL_Rect rect = {x * SQUARE_SIZE,
                             y * SQUARE_SIZE,
                             SQUARE_SIZE,
                             SQUARE_SIZE};
            SDL_SetRenderDrawColor(renderer, tilecolor.r, tilecolor.g, tilecolor.b, tilecolor.a);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Highlight if there is check
    if (board->isWhiteInCheck())
    {
        Coordinate pos = board->getWhiteKing()->getPosition();
        SDL_Rect rect = {pos.x * SQUARE_SIZE, pos.y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(renderer, 128, 0, 0, 200);
        SDL_RenderFillRect(renderer, &rect);
    }

    if (board->isBlackInCheck())
    {
        Coordinate pos = board->getBlackKing()->getPosition();
        SDL_Rect rect = {pos.x * SQUARE_SIZE, pos.y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(renderer, 128, 0, 0, 200);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Highlight selected piece(tile) and show valid moves
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

    // Render coordinate text
    for (int i = 0; i < 8; i++)
    {
        Texture coltexture(renderer);
        coltexture.loadChar(Game::columnlabel[i], SQUARE_SIZE / 5, color::BLUE);
        SDL_Rect colrect = {i * SQUARE_SIZE, 8 * SQUARE_SIZE - coltexture.getHeight(), coltexture.getWidth(), coltexture.getHeight()};
        coltexture.draw(NULL, &colrect);

        Texture rowtexture(renderer);
        rowtexture.loadChar(Game::rowlabel[7 - i], SQUARE_SIZE / 5, color::BLUE);
        SDL_Rect rowrect = {8 * SQUARE_SIZE - rowtexture.getWidth(), i * SQUARE_SIZE, rowtexture.getWidth(), rowtexture.getHeight()};
        rowtexture.draw(NULL, &rowrect);
    }

    // Draw pieces
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            SDL_Rect rect =
                {
                    x * SQUARE_SIZE,
                    y * SQUARE_SIZE,
                    SQUARE_SIZE,
                    SQUARE_SIZE,
                };
            if (board->getPieceAt({x, y}) != nullptr)
            {
                // TODO: add piece textures
                Texture font(renderer);
                font.loadChar(board->getPieceAt({x, y})->getSymbol(), SQUARE_SIZE, color::RED);
                SDL_Rect dest = {rect.x + (SQUARE_SIZE - font.getWidth()) / 2, rect.y + (SQUARE_SIZE - font.getHeight()) / 2, font.getWidth(), font.getHeight()};
                font.draw(NULL, &dest);
            }
        }
    }
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