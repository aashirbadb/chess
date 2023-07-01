#pragma once
#include "piece.h"
#include "board.h"
#include "move.h"

const Coordinate BISHOP_MOVE_DIRECTIONS[] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

class Bishop : public Piece
{
public:
    Bishop(Coordinate _pos, bool _isWhite);
    ~Bishop();
    std::vector<Move> getAllMoves(Board &_board) override;
    char getSymbol() override;
};
