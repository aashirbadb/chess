#include "headers/rook.h"

Rook::Rook(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

Rook::~Rook()
{
}

char Rook::getSymbol()
{
    return isColorWhite ? 'R' : 'r';
}

std::vector<Move> Rook::getAllMoves(Board &_board)
{
    std::vector<Move> moves;

    for (int i = 0; i < 4; i++)
    {
        Coordinate direction = ROOK_MOVE_DIRECTIONS[i];
        Coordinate next_position = {direction.x + position.x, direction.y + position.y};
        while (next_position.isValidPosition())
        {
            if (_board.getPieceAt(next_position) != nullptr)
            {
                if (isOpponentPieceAt(next_position, _board))
                {

                    moves.push_back(Move{.start = position, .end = next_position});
                }
                break;
            }

            moves.push_back(Move{.start = position, .end = next_position});

            next_position = {next_position.x + direction.x,
                             next_position.y + direction.y};
        }
    }

    return moves;
}