#pragma once
#include "board.h"
#include "piece.h"
#include "coordinate.h"

const Coordinate QUEEN_MOVE_DIRECTIONS[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

class Queen : public Piece
{
public:
    Queen(Coordinate _pos, bool _isWhite);
    ~Queen();
    std::vector<Move> getAllMoves(Board &_board) override;
    char getSymbol() override;
};
