
#include "headers/piece.h"
#include <iostream>
#include "headers/board.h"

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

std::vector<Move> Piece::getAllMoves(Board &_board)
{
    return std::vector<Move>();
}
char Piece::getSymbol()
{
    return '*'; // This should never be called. TODO: use throw here
}

bool Piece::isOpponentPieceAt(Coordinate _coord, Board &_board)
{
    Piece *_piece = _board.getPieceAt(_coord);
    if (_piece == nullptr)
    {
        return false;
    }
    else
    {
        return _board.getPieceAt(_coord)->isWhite() == !isColorWhite;
    }
}
