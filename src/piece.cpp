
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

bool Piece::isOpponentPieceAt(Coordinate _coord, Board *_board)
{
    Piece *_piece = _board->getPieceAt(_coord);
    if (_piece == nullptr)
    {
        return false;
    }
    else
    {
        return _board->getPieceAt(_coord)->isWhite() == !isColorWhite;
    }
}

bool Piece::isOwnPieceAt(Coordinate _coord, Board *_board)
{
    Piece *_piece = _board->getPieceAt(_coord);
    if (_piece == nullptr)
    {
        return false;
    }
    else
    {
        return _board->getPieceAt(_coord)->isWhite() == isColorWhite;
    }
}

std::vector<Move> Piece::getLegalMoves(Board &_board)
{
    std::vector<Move> allMoves = getAllMoves(_board);

    std::vector<Move> legalMoves;

    for (int i = 0; i < allMoves.size(); i++)
    {
        Board nextMoveBoardPosition(_board);
        nextMoveBoardPosition.moveUnchecked(allMoves[i]);
        if (!nextMoveBoardPosition.isPlayerInCheck())
        {
            legalMoves.push_back(allMoves[i]);
        }
    }

    return legalMoves;
}

void Piece::updateCoordinate(Coordinate _new_pos)
{
    position = _new_pos;
}