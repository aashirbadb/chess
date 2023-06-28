#include "headers/queen.h"

Queen::Queen(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

Queen::~Queen()
{
}

char Queen::getSymbol()
{
    return isColorWhite ? 'Q' : 'q';
}

std::vector<Move> Queen::getAllMoves()
{
    std::vector<Move> moves;

    return moves;
}