#pragma once
#include "piece.h"
#include "board.h"

class Pawn : public Piece
{

public:
    bool isFirstMove(); // Pawns can move 2 squares on their first move
    Pawn(Coordinate, bool);
    ~Pawn();
    std::vector<Move> getAllMoves(Board *) override;
    char getSymbol() override;
};