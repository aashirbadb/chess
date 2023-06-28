#pragma once
#include "piece.h"

class King : public Piece
{
public:
    King(Coordinate _pos, bool _isWhite);
    ~King();
    std::vector<Move> getAllMoves() override;
    char getSymbol() override;
};