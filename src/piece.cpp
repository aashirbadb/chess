
#include "headers/piece.h"
#include <iostream>

Piece::Piece(Coordinate _pos, bool _isWhite)
{
    position = _pos;
    isColorWhite = _isWhite;
}

Piece::~Piece()
{
}

Coordinate Piece::getPosition()
{
    return position;
}

bool Piece::isWhite()
{
    return isColorWhite;
}
std::vector<Move> Piece::getValidMoves() { return std::vector<Move>(); }

std::vector<Move> Piece::getAllMoves()
{
    return std::vector<Move>();
}
char Piece::getSymbol()
{
    return '*'; // This should never be called. TODO: use throw here
}
