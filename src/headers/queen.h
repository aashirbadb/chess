#pragma once
#include "piece.h"

class Queen : public Piece
{
public:
    Queen(Coordinate _pos, bool _isWhite);
    ~Queen();
    std::vector<Move> getAllMoves() override;
    char getSymbol() override;
};