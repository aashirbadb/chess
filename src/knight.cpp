#include "headers/knight.h"

Knight::Knight(Coordinate _pos, bool _isColorWhite) : Piece(_pos, _isColorWhite)
{
}

Knight::~Knight()
{
}

char Knight::getSymbol()
{
    return isColorWhite ? 'N' : 'n';
}

const Coordinate KNIGHT_MOVE_DIRECTIONS[] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

std::vector<Move> Knight::getAllMoves(Board &_board)
{
    std::vector<Move> moves;

    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + KNIGHT_MOVE_DIRECTIONS[i].x, position.y + KNIGHT_MOVE_DIRECTIONS[i].y};
        if (next_position.isValidPosition())
        {
            if (_board.getPieceAt(next_position) == nullptr || isOpponentPieceAt(next_position, _board))
            {
                moves.push_back(Move{.start = position, .end = next_position});
            }
        }
    }

    return moves;
}