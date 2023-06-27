#pragma once
#include <vector>
#include "coordinate.h"
#include "move.h"

class Piece
{
protected:
    Coordinate position;
    bool isColorWhite;

public:
    Piece(Coordinate _pos, bool _isWhite);
    ~Piece();

    Coordinate getPosition();
    bool isWhite();
    std::vector<Move> getValidMoves();

    // Get all moves, including invalid(cannot move due to check, blocking pieces) ones
    virtual std::vector<Move> getAllMoves();

    // Get the symbol of the piece(to show in the console)
    virtual char getSymbol();
};