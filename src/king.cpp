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
            if (isOpponentPieceAt(next_position, &_board))
            {
                moves.push_back(Move{position, next_position});
            }
            else if (_board.getPieceAt(next_position) == nullptr)
            {
                moves.push_back(Move{position, next_position});
            }
        }
    }

    // Generate castling moves
    // We only check if there are blocking pieces in the path here
    int tempy = isColorWhite ? 7 : 0;
    Coordinate kingStartPosition = {4, tempy};
    if (_board.castlingAvailable(isColorWhite ? 'K' : 'k') && position == kingStartPosition && _board.getPieceAt({7, tempy}))
    {
        Piece *a = _board.getPieceAt({5, tempy});
        Piece *b = _board.getPieceAt({6, tempy});

        if (a == nullptr && b == nullptr)
        {
            moves.push_back({{4, tempy}, {6, tempy}}); // not touching rook
            moves.push_back({{4, tempy}, {7, tempy}}); // touching rook
        }
    }

    if (_board.castlingAvailable(isColorWhite ? 'Q' : 'q') && position == kingStartPosition && _board.getPieceAt({0, tempy}))
    {
        Piece *a = _board.getPieceAt({1, tempy});
        Piece *b = _board.getPieceAt({2, tempy});
        Piece *c = _board.getPieceAt({3, tempy});

        if (a == nullptr && b == nullptr && c == nullptr)
        {
            moves.push_back({{4, tempy}, {2, tempy}}); // not touching rook
            moves.push_back({{4, tempy}, {0, tempy}}); // touching rook
        }
    }

    return moves;
}

static const Coordinate KING_CHECK_LOOPING[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
static const Coordinate KING_CHECK_KNIGHT[] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

bool King::isInCheck(Board *_board)
{

    for (int i = 0; i < 8; i++)
    {
        Coordinate direction = KING_CHECK_LOOPING[i];
        Coordinate next_position = {direction.x + position.x, direction.y + position.y};
        while (next_position.isValidPosition())
        {
            if (isOpponentPieceAt(next_position, _board))
            {
                return true;
            }
            else if (isOwnPieceAt(next_position, _board))
            {
                break;
            }

            next_position = {next_position.x + direction.x,
                             next_position.y + direction.y};
        }
    }

    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + KING_CHECK_KNIGHT[i].x, position.y + KING_CHECK_KNIGHT[i].y};
        if (next_position.isValidPosition())
        {
            if (isOpponentPieceAt(next_position, _board))
            {
                return true;
            }
        }
    }

    return false;
}
