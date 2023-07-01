#pragma once
#include "piece.h"
#include "board.h"

class Knight : public Piece
{
public:
    Knight(Coordinate _pos, bool _isWhite);
    ~Knight();
    std::vector<Move> getAllMoves(Board &_board) override;
    char getSymbol() override;
};