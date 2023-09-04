#pragma once
#include "board.h"
#include "piece.h"
#include "move.h"

class Piece;

class Bishop : public Piece
{
public:
    Bishop(Coordinate, bool);
    ~Bishop();
    std::vector<Move> getAllMoves(Board *) override;
    char getSymbol() override;

    static const Coordinate MOVE_DIRECTIONS[];
};
