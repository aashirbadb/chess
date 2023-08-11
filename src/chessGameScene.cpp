#include "headers/ChessGameScene.h"

char ChessGame::columnlabel[9] = "abcdefgh";
char ChessGame::rowlabel[9] = "12345678";

ChessGame::ChessGame(Game *g) : GameScene(g)
{
    game = g;
    board = new Board();
    selected_piece = nullptr;
}

ChessGame::~ChessGame()
{
    delete board;
}

void ChessGame::render()
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
            SDL_SetRenderDrawColor(game->getRenderer(), tilecolor.r, tilecolor.g, tilecolor.b, tilecolor.a);
            SDL_RenderFillRect(game->getRenderer(), &rect);
        }
    }

    // Highlight if there is check
    if (board->isWhiteInCheck())
    {
        Coordinate pos = board->getWhiteKing()->getPosition();
        SDL_Rect rect = {pos.x * SQUARE_SIZE, pos.y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(game->getRenderer(), 128, 0, 0, 200);
        SDL_RenderFillRect(game->getRenderer(), &rect);
    }

    if (board->isBlackInCheck())
    {
        Coordinate pos = board->getBlackKing()->getPosition();
        SDL_Rect rect = {pos.x * SQUARE_SIZE, pos.y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(game->getRenderer(), 128, 0, 0, 200);
        SDL_RenderFillRect(game->getRenderer(), &rect);
    }

    // Highlight selected piece(tile) and show valid moves
    if (selected_piece != nullptr)
    {
        Coordinate pos = selected_piece->getPosition();
        SDL_Rect rect = {pos.x * SQUARE_SIZE, pos.y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
        SDL_SetRenderDrawColor(game->getRenderer(), 0, 255, 0, 128);
        SDL_RenderFillRect(game->getRenderer(), &rect);

        std::vector<Move> moves = selected_piece->getLegalMoves(*board);

        for (int i = 0; i < moves.size(); i++)
        {
            SDL_Rect rect = {moves[i].end.x * SQUARE_SIZE, moves[i].end.y * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_SetRenderDrawColor(game->getRenderer(), 0, 255, 0, 64);
            SDL_RenderFillRect(game->getRenderer(), &rect);
        }
    }

    // Render coordinate text
    for (int i = 0; i < 8; i++)
    {
        Texture coltexture(game->getRenderer());
        coltexture.loadChar(columnlabel[i], SQUARE_SIZE / 5, color::BLUE);
        SDL_Rect colrect = {i * SQUARE_SIZE, 8 * SQUARE_SIZE - coltexture.getHeight(), coltexture.getWidth(), coltexture.getHeight()};
        coltexture.draw(NULL, &colrect);

        Texture rowtexture(game->getRenderer());
        rowtexture.loadChar(rowlabel[7 - i], SQUARE_SIZE / 5, color::BLUE);
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
                Texture font(game->getRenderer());
                font.loadChar(board->getPieceAt({x, y})->getSymbol(), SQUARE_SIZE, color::RED);
                SDL_Rect dest = {rect.x + (SQUARE_SIZE - font.getWidth()) / 2, rect.y + (SQUARE_SIZE - font.getHeight()) / 2, font.getWidth(), font.getHeight()};
                font.draw(NULL, &dest);
            }
        }
    }
}

#include "headers/gameMenu.h"

void ChessGame::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
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
                    game->requestRender();
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

        GameState state = board->getGameState();
        // TODO: Currently game just stops if the game ends
        if (
            (state == GameState::WhiteWins) || (state == GameState::BlackWins) || (state == GameState::Stalemate) || (state == GameState::Draw))
        {
            game->pushScene(new GameMenu(game));
        }
        game->requestRender();
    }
}
