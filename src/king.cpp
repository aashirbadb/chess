#include "headers/king.h"

King::King(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

King::~King()
{
}

char King::getSymbol()
{
    return isColorWhite ? 'K' : 'k';
}

const Coordinate KING_MOVE_DIRECTIONS[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

std::vector<Move> King::getAllMoves(Board &_board)
{
    std::vector<Move> moves;

    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + KING_MOVE_DIRECTIONS[i].x, position.y + KING_MOVE_DIRECTIONS[i].y};
        if (next_position.isValidPosition())
        {
            if (isOpponentPieceAt(next_position, _board))
            {
                moves.push_back(Move{position, next_position, MoveType::Capture});
            }
            else if (_board.getPieceAt(next_position) == nullptr)
            {
                moves.push_back(Move{position, next_position, MoveType::Normal});
            }
        }
    }

    // TODO: Generate moves for castling
    // Castling rules:
    // - The king is not in check
    // - The rook/king should not have moved
    // - Any path from king to rook should not be attacked by any piece
    // std::vector<Move> opponentAllMoves = _board.getAllPlayerMoves(!isColorWhite);

    return moves;
}