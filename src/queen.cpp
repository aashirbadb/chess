#include "headers/queen.h"

Queen::Queen(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

Queen::~Queen()
{
}

char Queen::getSymbol()
{
    return isColorWhite ? 'Q' : 'q';
}

std::vector<Move> Queen::getAllMoves(Board &_board)
{
    std::vector<Move> moves;

    for (int i = 0; i < 8; i++)
    {
        Coordinate direction = QUEEN_MOVE_DIRECTIONS[i];
        Coordinate next_position = {direction.x + position.x, direction.y + position.y};
        while (next_position.isValidPosition())
        {
            if (isOpponentPieceAt(next_position, &_board))
            {
                moves.push_back(Move{position, next_position});
                break;
            }
            else if (_board.getPieceAt(next_position) == nullptr)
            {
                moves.push_back(Move{position, next_position});
            }
            else
            {
                break;
            }

            next_position = {next_position.x + direction.x,
                             next_position.y + direction.y};
        }
    }

    return moves;
}