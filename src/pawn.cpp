#include "headers/pawn.h"

Pawn::Pawn(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
    if ((_pos.x == 1 && _isColorWhite) || (_pos.x == 6 && !_isColorWhite))
    {
        firstMove = true;
    }
    else
    {
        firstMove = false;
    }
}

Pawn::~Pawn()
{
}

char Pawn::getSymbol()
{
    return isColorWhite ? 'P' : 'p';
}

std::vector<Move> Pawn::getAllMoves()
{
    std::vector<Move> moves;
    int direction = isColorWhite ? 1 : -1;
    Coordinate nextPosition = {position.x + direction, position.y};
    if (nextPosition.isValidPosition())
    {
        Move move = {position, nextPosition};
        moves.push_back(move);
    }
    if (firstMove)
    {
        nextPosition = {position.x + 2 * direction, position.y};
        if (nextPosition.isValidPosition())
        {
            Move move = {position, nextPosition};
            moves.push_back(move);
        }
    }
    nextPosition = {position.x + direction, position.y + 1};
    if (nextPosition.isValidPosition())
    {
        Move move = {position, nextPosition};
        moves.push_back(move);
    }
    nextPosition = {position.x + direction, position.y - 1};
    if (nextPosition.isValidPosition())
    {
        Move move = {position, nextPosition};
        moves.push_back(move);
    }
    return moves;
}