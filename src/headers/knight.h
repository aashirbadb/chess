#pragma once
#include "piece.h"

class Knight : public Piece
{
public:
    Knight(Coordinate _pos, bool _isWhite);
    ~Knight();
    std::vector<Move> getAllMoves() override;
    char getSymbol() override;
};