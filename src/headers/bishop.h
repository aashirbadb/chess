#pragma once
#include "piece.h"
#include "board.h"
#include "move.h"

class Bishop : public Piece
{
public:
    Bishop(Coordinate _pos, bool _isWhite);
    ~Bishop();
    std::vector<Move> getAllMoves(Board &_board) override;
    char getSymbol() override;

    static const Coordinate MOVE_DIRECTIONS[];
};
