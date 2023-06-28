#include "headers/bishop.h"

Bishop::Bishop(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

Bishop::~Bishop()
{
}

char Bishop::getSymbol()
{
    return isColorWhite ? 'B' : 'b';
}

std::vector<Move> Bishop::getAllMoves()
{
    std::vector<Move> moves;

    return moves;
}