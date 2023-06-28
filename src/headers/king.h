#pragma once
#include "piece.h"

class King : public Piece
{
    bool firstMove; // king cannot move unless there is space and can move anywhere but one step only

public:
    King(Coordinate _pos, bool _isWhite);
    ~King();
    std::vector<Move> getAllMoves() override;
    char getSymbol() override;
};