#include "headers/King.h"

King::King(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
    if ((_pos.x == 1 && _isColorWhite) || (_pos.x == 6 && !_isColorWhite))
    {
        firstMove = true;
    }//this can be removed or what
    else
    {
        firstMove = false;
    }
}

King::~King()
{
}

char King::getSymbol()
{
    return isColorWhite ? 'K' : 'k';
}

std::vector<Move> King::getAllMoves()
{
    std::vector<Move> moves;
    int direction = isColorWhite ? 1 : -1;
    Coordinate nextPosition = {position.x + direction, position.y};
    if (nextPosition.isValidPosition())
    {
        Move move = {position, nextPosition};
        moves.push_back(move);
    }
   //King cannot move two times so removed
   //what about castling
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
    nextPosition = {position.x - direction, position.y + 1};// movement in backwards??
    if (nextPosition.isValidPosition())
    {
        Move move = {position, nextPosition};
        moves.push_back(move);
    }
    nextPosition = {position.x , position.y + 1};// movement in one side??
    if (nextPosition.isValidPosition())
    {
        Move move = {position, nextPosition};
        moves.push_back(move);
    }
    nextPosition = {position.x , position.y - 1};// movement in other side??
    if (nextPosition.isValidPosition())
    {
        Move move = {position, nextPosition};
        moves.push_back(move);
    }
    return moves;
}