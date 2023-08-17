#include <iostream>
#include "headers/ChessGameScene.h"

char ChessGame::columnlabel[9] = "abcdefgh";
char ChessGame::rowlabel[9] = "12345678";
char ChessGame::promotionPieces[4] = {'Q', 'R', 'B', 'N'};

ChessGame::ChessGame(Game *g) : GameScene(g)
{
    board = new Board("6r1/8/8/8/7K/8/2p2k2/8 w - - 0 65");
    selected_piece = nullptr;
    players[0] = new Human("Player 1");
    players[1] = new Human("Player 2");
    playing = true;
    loadTextures();
}

ChessGame::ChessGame(Game *g, bool isWhiteHuman, bool isBlackHuman) : GameScene(g)
{
    board = new Board();
    selected_piece = nullptr;
    if (isWhiteHuman)
        players[0] = new Human("Player 1");
    else
        players[0] = new Stockfish(10);

    if (isBlackHuman)
        players[1] = new Human("Player 2");
    else
        players[1] = new Stockfish(10);

    playing = true;
    loadTextures();
}

ChessGame::ChessGame(Game *g, bool isWhiteHuman, bool isBlackHuman, int difficulty) : GameScene(g)
{
    board = new Board();
    selected_piece = nullptr;
    if (isWhiteHuman)
        players[0] = new Human("Player 1");
    else
        players[0] = new Stockfish(difficulty);

    if (isBlackHuman)
        players[1] = new Human("Player 2");
    else
        players[1] = new Stockfish(difficulty);

    playing = true;
    loadTextures();
}

ChessGame::~ChessGame()
{
    while (!fileTexture.empty())
    {
        fileTexture.back()->~Texture();
        fileTexture.pop_back();
    }

    while (!rankTexture.empty())
    {
        rankTexture.back()->~Texture();
        rankTexture.pop_back();
    }

    delete board;
    delete players[0];
    delete players[1];
}

void ChessGame::render()
{
    SDL_SetRenderDrawColor(game->getRenderer(), color::WHITE.r, color::WHITE.g, color::WHITE.b, color::WHITE.a);
    SDL_RenderClear(game->getRenderer());

    WindowSize wsize = game->getWindowSize();

    renderBoard();

    if (board->isWaitingForPromotion() && getCurrentPlayer()->isHuman())
    {
        renderPromotionMenu();
    }

    int spacing = 10;
    int padding = 20;
    quitButtonRect = {
        spacing,
        wsize.height - quitTexture.getHeight() - spacing * 2 - 2 * padding,
        wsize.leftOffset - spacing * 2,
        quitTexture.getHeight() + padding * 2,
    };
    SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 200);
    SDL_RenderFillRect(game->getRenderer(), &quitButtonRect);
    quitTexture.drawCentered(quitButtonRect);

    if (state == GameState::Playing)
    {
        // Show resign button if game has not ended
        resignButtonRect = {
            spacing,
            quitButtonRect.y - quitButtonRect.h - spacing,
            wsize.leftOffset - spacing * 2,
            quitTexture.getHeight() + padding * 2,
        };
        replayButtonRect = {0, 0, 0, 0};

        SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 200);
        SDL_RenderFillRect(game->getRenderer(), &resignButtonRect);
        resignTexture.drawCentered(resignButtonRect);
    }
    else
    {
        // Show play again button and game result if game has ended
        replayButtonRect = {
            spacing,
            quitButtonRect.y - quitButtonRect.h - spacing,
            wsize.leftOffset - spacing * 2,
            quitTexture.getHeight() + padding * 2,
        };
        resignButtonRect = {0, 0, 0, 0};

        SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 200);
        SDL_RenderFillRect(game->getRenderer(), &replayButtonRect);
        replayTexture.drawCentered(replayButtonRect);
    }

    // Show names of players
    playerNameTextures[0].draw(wsize.leftOffset + wsize.boardSize + 10, wsize.height - playerNameTextures[0].getHeight() - 10);
    playerNameTextures[1].draw(wsize.leftOffset + wsize.boardSize + 10, 10);

    // Show captured pieces on the side
    std::vector<Piece *> whitecapturedpcs = board->getCapturedPieces(true);
    for (int i = 0; i < whitecapturedpcs.size(); i++)
    {
        char symbol = whitecapturedpcs[i]->getSymbol();

        int left = wsize.leftOffset + wsize.boardSize + 10;
        int totalWidth = wsize.width - left - 10;
        int width = (totalWidth) / 4;
        int height = width;
        int top = playerNameTextures[0].getHeight() + 10;

        SDL_Rect rect = {
            left + (i % 4) * width,
            top + (i / 4) * height,
            width,
            height,
        };
        pieceTextures[symbol].drawCentered(rect);
    }

    std::vector<Piece *> blackcapturedpcs = board->getCapturedPieces(false);
    for (int i = 0; i < blackcapturedpcs.size(); i++)
    {
        char symbol = blackcapturedpcs[i]->getSymbol();

        int left = wsize.leftOffset + wsize.boardSize + 10;
        int totalWidth = wsize.width - left - 10;
        int width = (totalWidth) / 4;
        int height = width;
        int bottom = wsize.height - height - playerNameTextures[0].getHeight() - 10;

        SDL_Rect rect = {
            left + (i % 4) * width,
            bottom - (i / 4) * height,
            width,
            height,
        };
        pieceTextures[symbol].drawCentered(rect);
    }

    SDL_Rect stateRect = {
        wsize.leftOffset + wsize.boardSize + 10,
        (wsize.height - stateTextures[state].getHeight()) / 2,
        wsize.width - (wsize.leftOffset + wsize.boardSize + 20),
        stateTextures[state].getHeight(),
    };
    stateTextures[state].drawCentered(stateRect);
}

void ChessGame::renderBoard()
{
    WindowSize wsize = game->getWindowSize();

    // Draw board tiles
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            SDL_Color tilecolor = board->getBoardColorAt(x, y) ? color::WHITE_TILE : color::BLACK_TILE;
            renderTile({x, y}, tilecolor);
        }
    }

    // Highlight last move
    std::vector<Move> prevmoves = board->getPreviousMoves();
    if (prevmoves.size() > 0)
    {
        Move prevmv = prevmoves.back();
        renderTile({prevmv.start.x, prevmv.start.y}, color::PREV_MOVE_TILE);
        renderTile({prevmv.end.x, prevmv.end.y}, color::PREV_MOVE_TILE);
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
            int r = wsize.tileSize / 5;
            int x = wsize.leftOffset + wsize.tileSize * moves[i].end.x + wsize.tileSize / 2;
            int y = wsize.topOffset + wsize.tileSize * moves[i].end.y + wsize.tileSize / 2;

            if (board->getPieceAt(moves[i].end) == nullptr)
                Texture::drawCircle(game->getRenderer(), x, y, r, color::GREEN);
            else
                renderTile(moves[i].end, color::CAPTURE_TILE);
        }
    }

    // Render coordinate text
    for (int i = 0; i < 8; i++)
    {
        SDL_Rect filerect = {
            i * wsize.tileSize + wsize.leftOffset + 2,
            wsize.boardSize + wsize.topOffset - fileTexture[i]->getHeight(),
            fileTexture[i]->getWidth(),
            fileTexture[i]->getHeight(),
        };

        SDL_Rect rankrect = {
            wsize.boardSize - rankTexture[i]->getWidth() + wsize.leftOffset - 2,
            i * wsize.tileSize + wsize.topOffset + 2,
            rankTexture[i]->getWidth(),
            rankTexture[i]->getHeight(),
        };

        fileTexture[i]->draw(NULL, &filerect);
        rankTexture[i]->draw(NULL, &rankrect);
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
}

void ChessGame::renderPromotionMenu()
{
    WindowSize wsize = game->getWindowSize();
    // Show promotion menu
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

        pieceTextures[whitePromotion ? toupper(promotionPieces[i]) : tolower(promotionPieces[i])].draw(NULL, &rect);
    }
}

void ChessGame::handleEvent(SDL_Event &e)
{
    WindowSize wsize = game->getWindowSize();

    Player *currentPlayer = getCurrentPlayer();

    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (playing && currentPlayer->isHuman() && board->isWaitingForPromotion())
        {
            handlePromotion(x, y);
        }
        else if (playing && currentPlayer->isHuman() && hasClickedInsideButton(x, y, {wsize.leftOffset, wsize.topOffset, wsize.boardSize, wsize.boardSize}))
        {
            // If player has clicked inside the board
            Coordinate board_coord{(x - wsize.leftOffset) / wsize.tileSize, (y - wsize.topOffset) / wsize.tileSize};
            handleBoardClick(board_coord);
        }

        if (hasClickedInsideButton(x, y, quitButtonRect))
        {
            game->playSound(Sound::ButtonClick);
            game->pushScene(new GameMenu(game));
        }
        else if (hasClickedInsideButton(x, y, resignButtonRect) && currentPlayer->isHuman())
        {
            board->resign();
        }
        else if (hasClickedInsideButton(x, y, replayButtonRect))
        {
            bool p1 = players[0]->isHuman();
            bool p2 = players[0]->isHuman();

            game->playSound(Sound::ButtonClick);
            game->pushScene(new ChessGame(game, players[1]->isHuman(), players[0]->isHuman()));
        }
    }
}

void ChessGame::renderTile(Coordinate coord, SDL_Color color)
{
    WindowSize wsize = game->getWindowSize();
    SDL_Rect rect = {coord.x * wsize.tileSize + wsize.leftOffset, coord.y * wsize.tileSize + wsize.topOffset, wsize.tileSize, wsize.tileSize};
    SDL_SetRenderDrawColor(game->getRenderer(), color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(game->getRenderer(), &rect);
}

void ChessGame::renderPiece(Piece *piece)
{
    WindowSize wsize = game->getWindowSize();

    SDL_Rect rect = {piece->getPosition().x * wsize.tileSize + wsize.leftOffset, piece->getPosition().y * wsize.tileSize + wsize.topOffset, wsize.tileSize, wsize.tileSize};
    pieceTextures[piece->getSymbol()].drawCentered(rect);
}

void ChessGame::handlePromotion(int x, int y)
{
    WindowSize wsize = game->getWindowSize();
    bool whitePromotion = board->getPromotionPiece()->isWhite();
    for (int i = 0; i < 4; i++)
    {
        SDL_Rect rect = {wsize.boardSize / 2 - (2 - i) * wsize.tileSize + wsize.leftOffset, wsize.boardSize / 2 - wsize.tileSize / 2, wsize.tileSize, wsize.tileSize};
        if (hasClickedInsideButton(x, y, rect))
        {
            board->promoteTo(whitePromotion ? toupper(promotionPieces[i]) : tolower(promotionPieces[i]));
            game->playSound(Sound::Promote);
            break;
        }
    }
}

void ChessGame::handleBoardClick(Coordinate board_coord)
{
    if (!getCurrentPlayer()->isHuman())
        return;

    WindowSize wsize = game->getWindowSize();
    Piece *piece = board->getPieceAt(board_coord);

    if (selected_piece != nullptr)
    {
        std::vector<Move> moves = selected_piece->getLegalMoves(*board);

        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i].end == board_coord)
            {
                MoveType mvtype = board->performMove(moves[i]);
                selected_piece = nullptr;

                playSound(mvtype);
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

void ChessGame::playSound(MoveType mvtype)
{
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

    if (board->isWhiteInCheck() || board->isBlackInCheck())
    {
        game->playSound(Sound::Check);
        return;
    }
}

void ChessGame::update()
{
    if (!playing)
        return;
    Player *currentplayer = getCurrentPlayer();
    if (!currentplayer->isHuman())
    {
        Move mv = currentplayer->getMove(board);

        MoveType mvtype = board->performMove(mv);
        playSound(mvtype);
    }

    state = board->getGameState();
    if (state != GameState::Playing)
    {
        playing = false;
        switch (state)
        {
        case GameState::WhiteWins:
        case GameState::BlackWins:
            game->playSound(Sound::Victory);
            break;
        case GameState::Draw:
        case GameState::Stalemate:
        case GameState::WhiteResigns:
        case GameState::BlackResigns:
            game->playSound(Sound::Draw);
            break;
        default:
            break;
        }
    }
}

Player *ChessGame::getCurrentPlayer()
{
    int playeridx = board->getIsWhiteTurn() ? 0 : 1;
    return players[playeridx];
}

void ChessGame::handleResize()
{
    loadTextures();
}

void ChessGame::loadTextures()
{
    WindowSize wsize = game->getWindowSize();
    SDL_Renderer *renderer = game->getRenderer();

    // Quit button texture
    quitButtonRect = {0, 0, 0, 0};
    quitTexture.setRenderer(renderer);
    quitTexture.loadString("Quit", 24, color::RED);

    // Resign button texture
    resignButtonRect = {0, 0, 0, 0};
    resignTexture.setRenderer(renderer);
    resignTexture.loadString("Resign", 24, color::RED);

    // Play again
    replayButtonRect = {0, 0, 0, 0};
    replayTexture.setRenderer(renderer);
    replayTexture.loadString("Play Again", 24, color::RED);

    // Player names
    playerNameTextures[0].setRenderer(renderer);
    playerNameTextures[0].loadString(players[0]->getName(), 48, color::BLACK);

    playerNameTextures[1].setRenderer(renderer);
    playerNameTextures[1].loadString(players[1]->getName(), 48, color::BLACK);

    int fontsize = (wsize.width - wsize.leftOffset - wsize.boardSize - 20) / 4;
    const char pieceSymbol[7] = "BKNPQR";
    for (int i = 0; i < 6; i++)
    {
        char upper = toupper(pieceSymbol[i]);
        char lower = tolower(pieceSymbol[i]);

        pieceTextures[upper].setRenderer(renderer);

        pieceTextures[upper].loadImage("assets/w" + std::string(1, pieceSymbol[i]) + ".svg");

        pieceTextures[lower].setRenderer(renderer);
        pieceTextures[lower].loadImage("assets/b" + std::string(1, pieceSymbol[i]) + ".svg");
    }

    for (int i = 0; i < 8; i++)
    {
        Texture *rankt = new Texture(renderer);
        rankt->loadChar('8' - i, wsize.tileSize / 5, board->getBoardColorAt(i, 7) ? color::BLACK_TILE : color::WHITE_TILE);
        rankTexture.push_back(rankt);

        Texture *filet = new Texture(renderer);
        filet->loadChar('a' + i, wsize.tileSize / 5, board->getBoardColorAt(7, i) ? color::BLACK_TILE : color::WHITE_TILE);
        fileTexture.push_back(filet);
    }

    GameState states[] = {
        GameState::Playing,
        GameState::Stalemate,
        GameState::WhiteWins,
        GameState::BlackWins,
        GameState::WhiteResigns,
        GameState::BlackResigns,
        GameState::Draw,
    };

    std::string stateString[] = {
        " ",
        "Stalemate!!!",
        "White wins!!!",
        "Black wins!!!",
        "White resigns!!!",
        "Black resigns!!!",
        "Draw!!!",
    };

    for (int i = 0; i < 6; i++)
    {
        stateTextures[states[i]].setRenderer(renderer);
        stateTextures[states[i]].loadString(stateString[i], 24, color::BLACK);
    }
}