#pragma once
#include "gameScene.h"
#include "board.h"
#include "piece.h"
#include "stockfish.h"
#include "player.h"

class ChessGame : public GameScene
{
    Board *board;
    Piece *selected_piece;
    static char rowlabel[9], columnlabel[9], promotionPieces[4];
    Player *players[2];
    GameState state;

public:
    ChessGame(Game *g);
    ChessGame(Game *g, bool isWhiteHuman, bool isBlackHuman);
    ChessGame(Game *g, bool isWhiteHuman, bool isBlackHuman, int difficulty);

    ~ChessGame() override;
    void render() override;
    void handleEvent(SDL_Event &e) override;
    void update() override;

private:
    void renderTile(int x, int y, SDL_Color color);
    void renderTile(Coordinate coord, SDL_Color color);
    void renderPiece(Piece *piece);
    void handlePromotion(int x, int y);
    void handleBoardClick(Coordinate);

    void playSounds(MoveType _type);

    Player *getCurrentPlayer();
};
