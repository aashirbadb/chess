#pragma once
#include "piece.h"
#include "board.h"

const Coordinate ROOK_MOVE_DIRECTIONS[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

class Rook : public Piece
{
public:
    Rook(Coordinate _pos, bool _isWhite);
    ~Rook();
    std::vector<Move> getAllMoves(Board &_board) override;
    char getSymbol() override;
};