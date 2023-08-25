#pragma once
#include "board.h"
#include "piece.h"
#include "coordinate.h"

class Queen : public Piece
{
public:
    Queen(Coordinate _pos, bool _isWhite);
    ~Queen();
    std::vector<Move> getAllMoves(Board &_board) override;
    char getSymbol() override;

    static const Coordinate MOVE_DIRECTIONS[];
};
