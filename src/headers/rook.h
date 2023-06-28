#pragma once
#include "piece.h"

class Rook : public Piece
{
public:
    Rook(Coordinate _pos, bool _isWhite);
    ~Rook();
    std::vector<Move> getAllMoves() override;
    char getSymbol() override;
};