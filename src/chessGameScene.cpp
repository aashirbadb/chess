#include <iostream>
#include "headers/ChessGameScene.h"
#include "headers/gameover.h"

char ChessGame::columnlabel[9] = "abcdefgh";
char ChessGame::rowlabel[9] = "12345678";
char ChessGame::promotionPieces[4] = {'Q', 'R', 'B', 'K'};

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
            SDL_Color tilecolor = board->getBoardColorAt(x, y) ? color::WHITE_TILE : color::BLACK_TILE;
            renderTile(x, y, tilecolor);
        }
    }

    // Highlight if there is check
    if (board->isWhiteInCheck())
    {
        Coordinate pos = board->getWhiteKing()->getPosition();
        renderTile(pos, color::CHECK_TILE);
    }

    if (board->isBlackInCheck())
    {
        Coordinate pos = board->getBlackKing()->getPosition();
        renderTile(pos, color::CHECK_TILE);
    }

    // Highlight selected piece(tile) and show valid moves
    if (selected_piece != nullptr)
    {
        Coordinate pos = selected_piece->getPosition();
        renderTile(pos, color::SELECTED_TILE);
        std::vector<Move> moves = selected_piece->getLegalMoves(*board);

        for (int i = 0; i < moves.size(); i++)
        {
            renderTile(moves[i].end, color::MOVE_TILE);
        }
    }

    WindowSize wsize = game->getWindowSize();
    // Render coordinate text
    for (int i = 0; i < 8; i++)
    {
        Texture coltexture(game->getRenderer());
        coltexture.loadChar(columnlabel[i], wsize.tileSize / 5, color::BLUE);
        SDL_Rect colrect = {
            i * wsize.tileSize + wsize.leftOffset + 2,
            wsize.boardSize + wsize.topOffset - coltexture.getHeight(),
            coltexture.getWidth(),
            coltexture.getHeight(),
        };
        coltexture.draw(NULL, &colrect);

        Texture rowtexture(game->getRenderer());
        rowtexture.loadChar(rowlabel[7 - i], wsize.tileSize / 5, color::BLUE);
        SDL_Rect rowrect = {
            wsize.boardSize - rowtexture.getWidth() + wsize.leftOffset - 2,
            i * wsize.tileSize + wsize.topOffset + 2,
            rowtexture.getWidth(),
            rowtexture.getHeight(),
        };
        rowtexture.draw(NULL, &rowrect);
    }

    // Draw pieces
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (board->getPieceAt({x, y}) != nullptr)
            {
                renderPiece(board->getPieceAt({x, y}));
            }
        }
    }

    // Show promotion menu
    if (board->isWaitingForPromotion())
    {
        bool whitePromotion = board->getPromotionPiece()->isWhite();
        for (int i = 0; i < 4; i++)
        {
            SDL_Rect rect = {wsize.boardSize / 2 - (2 - i) * wsize.tileSize + wsize.leftOffset, wsize.boardSize / 2 - wsize.tileSize / 2, wsize.tileSize, wsize.tileSize};
            SDL_Color rect_bg;
            if (i % 2 == 0)
                rect_bg = color::GREEN;
            else
                rect_bg = color::BLUE;

            SDL_SetRenderDrawColor(game->getRenderer(), rect_bg.r, rect_bg.g, rect_bg.b, rect_bg.a);

            SDL_RenderFillRect(game->getRenderer(), &rect);

            Texture text(game->getRenderer());
            text.loadChar(whitePromotion ? toupper(promotionPieces[i]) : tolower(promotionPieces[i]), wsize.tileSize, color::BLACK);
            text.draw(NULL, &rect);
        }
    }
}

void ChessGame::handleEvent(SDL_Event &e)
{
    WindowSize wsize = game->getWindowSize();
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (board->isWaitingForPromotion())
        {
            bool whitePromotion = board->getPromotionPiece()->isWhite();
            for (int i = 0; i < 4; i++)
            {
                SDL_Rect rect = {wsize.boardSize / 2 - (2 - i) * wsize.tileSize + wsize.leftOffset, wsize.boardSize / 2 - wsize.tileSize / 2, wsize.tileSize, wsize.tileSize};
                if (hasClickedInsideButton(x, y, rect))
                {
                    board->promoteTo(whitePromotion ? toupper(promotionPieces[i]) : tolower(promotionPieces[i]));
                    game->playSound(Sound::Promote);
                    game->requestRender();
                    break;
                }
            }
        }
        else
        {
            Coordinate selected_coord{(x - wsize.leftOffset) / wsize.tileSize, y / wsize.tileSize};
            Piece *piece = board->getPieceAt(selected_coord);

            if (selected_piece != nullptr)
            {
                std::vector<Move> moves = selected_piece->getLegalMoves(*board);

                for (int i = 0; i < moves.size(); i++)
                {
                    if (moves[i].end == selected_coord)
                    {
                        MoveType mvtype = board->performMove(moves[i]);
                        selected_piece = nullptr;
                        game->requestRender();

                        switch (mvtype)
                        {
                        case MoveType::Enpassant:
                        case MoveType::Normal:
                            game->playSound(Sound::Move);
                            break;
                        case MoveType::Promotion:
                            game->playSound(Sound::Promote);
                            break;
                        case MoveType::Capture:
                            game->playSound(Sound::Capture);
                            break;
                        case MoveType::Castle:
                            game->playSound(Sound::Castle);
                            break;

                        default:
                            break;
                        }

                        GameState state = board->getGameState();

                        if (state != GameState::Playing)
                        {
                            switch (state)
                            {
                            case GameState::WhiteWins:
                            case GameState::BlackWins:
                                game->playSound(Sound::Victory);
                                break;
                            case GameState::Draw:
                            case GameState::Stalemate:
                                game->playSound(Sound::Draw);
                                break;
                            default:
                                break;
                            }
                            game->pushScene(new GameOver(game, state));
                        }

                        if (board->isWhiteInCheck() || board->isBlackInCheck())
                        {
                            game->playSound(Sound::Check);
                            return;
                        }

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
        }

        game->requestRender();
    }
}

void ChessGame::renderTile(int x, int y, SDL_Color color)
{
    WindowSize wsize = game->getWindowSize();
    SDL_Rect rect = {x * wsize.tileSize + wsize.leftOffset, y * wsize.tileSize + wsize.topOffset, wsize.tileSize, wsize.tileSize};
    SDL_SetRenderDrawColor(game->getRenderer(), color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(game->getRenderer(), &rect);
}

void ChessGame::renderTile(Coordinate coord, SDL_Color color)
{
    renderTile(coord.x, coord.y, color);
}

void ChessGame::renderPiece(Piece *piece)
{
    WindowSize wsize = game->getWindowSize();

    SDL_Rect rect = {piece->getPosition().x * wsize.tileSize + wsize.leftOffset, piece->getPosition().y * wsize.tileSize + wsize.topOffset, wsize.tileSize, wsize.tileSize};
    Texture font(game->getRenderer());
    font.loadChar(piece->getSymbol(), wsize.tileSize, color::RED);
    // used to center the font
    SDL_Rect dest = {rect.x + (wsize.tileSize - font.getWidth()) / 2, rect.y + (wsize.tileSize - font.getHeight()) / 2, font.getWidth(), font.getHeight()};
    font.draw(NULL, &dest);
}