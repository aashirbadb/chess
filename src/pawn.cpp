#include "headers/pawn.h"

Pawn::Pawn(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
    if ((_pos.x == 6 && _isColorWhite) || (_pos.x == 1 && !_isColorWhite))
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

std::vector<Move> Pawn::getAllMoves(Board &_board)
{
    std::vector<Move> moves;
    int direction = isColorWhite ? -1 : 1;
    Coordinate oneahead = {position.x + direction, position.y};
    Coordinate twoahead = {position.x + direction * 2, position.y};
    bool oneaheadallowed = false;

    // One move ahead
    if (oneahead.isValidPosition() && _board.getPieceAt(oneahead) == nullptr)
    {
        moves.push_back({position, oneahead, oneahead.isPromotionSquare(isColorWhite) ? moveType::Promotion : moveType::Normal});
        oneaheadallowed = true;
    }

    // Two moves on firstmove
    if (firstMove && oneaheadallowed && twoahead.isValidPosition() && _board.getPieceAt(twoahead) == nullptr)
    {
        moves.push_back({position, twoahead, moveType::PawnFirstMove});
    }

    // Capture on right
    Coordinate nextMove = {position.x + direction, position.y + 1};
    if (nextMove.isValidPosition() && isOpponentPieceAt(nextMove, _board))
    {
        moves.push_back({position, nextMove, nextMove.isPromotionSquare(isColorWhite) ? moveType::Promotion : moveType::Capture});
        _board.setEnpassantTarget(oneahead);
    }

    // Capture on left
    nextMove = {position.x + direction, position.y - 1};
    if (nextMove.isValidPosition() && isOpponentPieceAt(nextMove, _board))
    {
        moves.push_back({position, nextMove, nextMove.isPromotionSquare(isColorWhite) ? moveType::Promotion : moveType::Capture});
        _board.setEnpassantTarget(oneahead);
    }

    // TODO: Generate move for enpassant
    Coordinate enPassantTarget = _board.getEnpassantTarget();
    if (enPassantTarget.isValidPosition())
    {
        Coordinate enPassantPawn1 = {enPassantTarget.x - direction, enPassantTarget.y + 1};
        Coordinate enPassantPawn2 = {enPassantTarget.x - direction, enPassantTarget.y - 1};
    }

    return moves;
}
