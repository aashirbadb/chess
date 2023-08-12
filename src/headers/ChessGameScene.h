#pragma once
#include "gameScene.h"
#include "board.h"
#include "piece.h"

class ChessGame : public GameScene
{
    Board *board;
    Piece *selected_piece;
    static char rowlabel[9], columnlabel[9], promotionPieces[4];

public:
    ChessGame(Game *g);
    ~ChessGame() override;
    void render() override;
    void handleEvent(SDL_Event &e) override;
};