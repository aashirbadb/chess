#include "headers/knight.h"

Knight::Knight(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

Knight::~Knight()
{
}

char Knight::getSymbol()
{
    return isColorWhite ? 'N' : 'n';
}

std::vector<Move> Knight::getAllMoves()
{
    std::vector<Move> moves;

    return moves;
}