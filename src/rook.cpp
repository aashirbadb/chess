#include "headers/rook.h"

Rook::Rook(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

Rook::~Rook()
{
}

char Rook::getSymbol()
{
    return isColorWhite ? 'R' : 'r';
}

std::vector<Move> Rook::getAllMoves()
{
    std::vector<Move> moves;

    return moves;
}