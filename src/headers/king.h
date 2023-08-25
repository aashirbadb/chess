#pragma once
#include "piece.h"
#include "board.h"

class King : public Piece
{
public:
    King(Coordinate _pos, bool _isWhite);
    ~King();
    std::vector<Move> getAllMoves(Board &_board) override;
    char getSymbol() override;

    bool isInCheck(Board *_board);
    
    static const Coordinate MOVE_DIRECTIONS[];
};