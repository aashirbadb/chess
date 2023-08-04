#include "headers/bishop.h"

Bishop::Bishop(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

Bishop::~Bishop()
{
}

char Bishop::getSymbol()
{
    return isColorWhite ? 'B' : 'b';
}

std::vector<Move> Bishop::getAllMoves(Board &_board)
{
    std::vector<Move> moves;

    for (int i = 0; i < 4; i++)
    {
        Coordinate direction = BISHOP_MOVE_DIRECTIONS[i];
        Coordinate next_position = {direction.x + position.x, direction.y + position.y};
        while (next_position.isValidPosition())
        {
            if (isOpponentPieceAt(next_position, _board))
            {
                moves.push_back(Move{position, next_position, MoveType::Capture});
                break;
            }
            else if (_board.getPieceAt(next_position) == nullptr)
            {
                moves.push_back(Move{position, next_position, MoveType::Normal});
            }else{
                break;
            }

            next_position = {next_position.x + direction.x,
                             next_position.y + direction.y};
        }
    }

    return moves;
}