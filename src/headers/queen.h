#pragma once
#include "piece.h"

class Queen : public Piece
{
    bool firstMove; // queen cannot move unless there is space and can move anywhere but one step only

public:
    Queen(Coordinate _pos, bool _isWhite);
    ~Queen();
    std::vector<Move> getAllMoves() override;
    char getSymbol() override;
};