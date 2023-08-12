#include "headers/pawn.h"

Pawn::Pawn(Coordinate _pos, bool _isWhite) : Piece(_pos, _isWhite) {}

inline bool Pawn::isFirstMove()
{
    return ((position.y == 6 && isColorWhite) || (position.y == 1 && !isColorWhite));
}

Pawn::~Pawn()
{
}

char Pawn::getSymbol()
{
    return isColorWhite ? 'P' : 'p';
}

std::vector<Move> Pawn::getAllMoves(Board &_board)
{
    std::vector<Move> moves;
    int direction = isColorWhite ? -1 : 1;
    Coordinate oneahead = {position.x, position.y + direction};
    Coordinate twoahead = {position.x, position.y + direction * 2};
    bool oneaheadallowed = false;

    // One move ahead
    if (oneahead.isValidPosition() && _board.getPieceAt(oneahead) == nullptr)
    {
        moves.push_back({position, oneahead});
        oneaheadallowed = true;
    }

    // Two moves on firstmove
    if (isFirstMove() && oneaheadallowed && twoahead.isValidPosition() && _board.getPieceAt(twoahead) == nullptr)
    {
        moves.push_back({position, twoahead});
    }

    // Capture on right
    Coordinate nextMove = {position.x + 1, position.y + direction};
    if (nextMove.isValidPosition() && isOpponentPieceAt(nextMove, &_board))
    {
        moves.push_back({position, nextMove});
        _board.setEnpassantTarget(oneahead);
    }

    // Capture on left
    nextMove = {position.x - 1, position.y + direction};
    if (nextMove.isValidPosition() && isOpponentPieceAt(nextMove, &_board))
    {
        moves.push_back({position, nextMove});
        _board.setEnpassantTarget(oneahead);
    }

    // TODO: Generate move for enpassant
    // Coordinate enPassantTarget = _board.getEnpassantTarget();
    // if (enPassantTarget.isValidPosition())
    // {
    //     Coordinate enPassantPawn1 = {enPassantTarget.x - direction, enPassantTarget.y + 1};
    //     Coordinate enPassantPawn2 = {enPassantTarget.x - direction, enPassantTarget.y - 1};
    // }

    return moves;
}
