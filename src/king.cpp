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
                moves.push_back(Move{position, next_position, moveType::Capture});
            }
            else if (_board.getPieceAt(next_position) == nullptr)
            {
                moves.push_back(Move{position, next_position, moveType::Normal});
            }
        }
    }

    // TODO: Generate moves for castling

    return moves;
}