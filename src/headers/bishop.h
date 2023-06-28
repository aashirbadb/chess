#pragma once
#include "piece.h"

class Bishop : public Piece
{
public:
    Bishop(Coordinate _pos, bool _isWhite);
    ~Bishop();
    std::vector<Move> getAllMoves() override;
    char getSymbol() override;
};