#include "headers/knight.h"

const Coordinate Knight::MOVE_DIRECTIONS[] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

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

std::vector<Move> Knight::getAllMoves(Board *_board)
{
    std::vector<Move> moves;

    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + Knight::MOVE_DIRECTIONS[i].x, position.y + Knight::MOVE_DIRECTIONS[i].y};
        if (next_position.isValidPosition())
        {
            if (isOpponentPieceAt(next_position, _board))
            {
                moves.push_back({position, next_position, MOVETYPE_CAPTURE});
            }
            else if ((_board->getPieceAt(next_position) == nullptr))
            {
                moves.push_back({position, next_position, MOVETYPE_NONE});
            }
        }
    }

    return moves;
}