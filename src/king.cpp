#include "headers/king.h"
#include "headers/board.h"
#include "headers/knight.h"
#include "headers/rook.h"
#include "headers/bishop.h"

const Coordinate King::MOVE_DIRECTIONS[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

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

std::vector<Move> King::getAllMoves(Board *_board)
{
    std::vector<Move> moves;

    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + King::MOVE_DIRECTIONS[i].x, position.y + King::MOVE_DIRECTIONS[i].y};
        if (next_position.isValidPosition())
        {
            if (isOpponentPieceAt(next_position, _board))
            {
                moves.push_back(Move{position, next_position, MOVETYPE_CAPTURE});
            }
            else if (_board->getPieceAt(next_position) == nullptr)
            {
                moves.push_back(Move{position, next_position, MOVETYPE_NONE});
            }
        }
    }

    // Generate castling moves
    // We only check if there are blocking pieces in the path here
    int tempy = isColorWhite ? 7 : 0;
    Coordinate kingStartPosition = {4, tempy};
    std::set<Coordinate> checkCoordinates = getCheckCoordinates(_board);
    if (_board->castlingAvailable(isColorWhite ? 'K' : 'k') && position == kingStartPosition && _board->getPieceAt({7, tempy}))
    {
        Coordinate ap{5, tempy}, bp{6, tempy};
        Piece *a = _board->getPieceAt(ap);
        Piece *b = _board->getPieceAt(bp);

        if (a == nullptr &&
            b == nullptr &&
            checkCoordinates.find(ap) == checkCoordinates.end() &&
            checkCoordinates.find(bp) == checkCoordinates.end())
        {
            moves.push_back({{4, tempy}, {6, tempy}, MOVETYPE_KINGSIDE_CASTLE}); // not touching rook
            moves.push_back({{4, tempy}, {7, tempy}, MOVETYPE_KINGSIDE_CASTLE}); // touching rook
        }
    }

    if (_board->castlingAvailable(isColorWhite ? 'Q' : 'q') && position == kingStartPosition && _board->getPieceAt({0, tempy}))
    {
        Coordinate ap{1, tempy}, bp{2, tempy}, cp{3, tempy};

        Piece *a = _board->getPieceAt(ap);
        Piece *b = _board->getPieceAt(bp);
        Piece *c = _board->getPieceAt(cp);

        if (a == nullptr &&
            b == nullptr &&
            c == nullptr &&
            checkCoordinates.find(ap) == checkCoordinates.end() &&
            checkCoordinates.find(bp) == checkCoordinates.end() &&
            checkCoordinates.find(cp) == checkCoordinates.end())
        {
            moves.push_back({{4, tempy}, {2, tempy}, MOVETYPE_QUEENSIDE_CASTLE}); // not touching rook
            moves.push_back({{4, tempy}, {0, tempy}, MOVETYPE_QUEENSIDE_CASTLE}); // touching rook
        }
    }

    return moves;
}

bool King::isInCheck(Board *_board)
{
    // Check for pawns
    Coordinate pawn1 = {position.x + 1, position.y + (isColorWhite ? -1 : 1)};
    if (pawn1.isValidPosition() && isOpponentPieceAt(pawn1, _board) && _board->getPieceAt(pawn1)->isPawn())
    {
        return true;
    }
    Coordinate pawn2 = {position.x - 1, position.y + (isColorWhite ? -1 : 1)};
    if (pawn2.isValidPosition() && isOpponentPieceAt(pawn2, _board) && _board->getPieceAt(pawn2)->isPawn())
    {
        return true;
    }

    // Check for knights
    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + Knight::MOVE_DIRECTIONS[i].x, position.y + Knight::MOVE_DIRECTIONS[i].y};
        if (next_position.isValidPosition() && isOpponentPieceAt(next_position, _board) && _board->getPieceAt(next_position)->isKnight())
        {
            return true;
        }
    }

    // Check for opponent king
    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + King::MOVE_DIRECTIONS[i].x, position.y + King::MOVE_DIRECTIONS[i].y};
        if (next_position.isValidPosition() && isOpponentPieceAt(next_position, _board) && _board->getPieceAt(next_position)->isKing())
        {
            return true;
        }
    }

    // Check on diagonals(bishop and queen)
    for (int i = 0; i < 4; i++)
    {
        Coordinate direction = Bishop::MOVE_DIRECTIONS[i];
        Coordinate next_position = {direction.x + position.x, direction.y + position.y};
        while (next_position.isValidPosition())
        {
            Piece *pc = _board->getPieceAt(next_position);
            char sy = tolower(pc != nullptr ? pc->getSymbol() : '\0');
            if (isOpponentPieceAt(next_position, _board) && ((sy == 'q') || (sy == 'b')))
            {
                return true;
            }
            else if (pc != nullptr)
            {
                break;
            }

            next_position = {next_position.x + direction.x,
                             next_position.y + direction.y};
        }
    }

    // Check straight lines(rook and queen)
    for (int i = 0; i < 4; i++)
    {
        Coordinate direction = Rook::MOVE_DIRECTIONS[i];
        Coordinate next_position = {direction.x + position.x, direction.y + position.y};
        while (next_position.isValidPosition())
        {
            Piece *pc = _board->getPieceAt(next_position);
            char sy = tolower(pc != nullptr ? pc->getSymbol() : '\0');
            if (isOpponentPieceAt(next_position, _board) && ((sy == 'r') || (sy == 'q')))
            {
                return true;
            }
            else if (pc != nullptr)
            {
                break;
            }

            next_position = {next_position.x + direction.x,
                             next_position.y + direction.y};
        }
    }

    return false;
}

std::set<Coordinate> King::getCheckCoordinates(Board *_board)
{
    std::set<Coordinate> coordinates;
    // Check for pawns
    Coordinate pawn1 = {position.x + 1, position.y + (isColorWhite ? -1 : 1)};
    if (pawn1.isValidPosition() && isOpponentPieceAt(pawn1, _board) && _board->getPieceAt(pawn1)->isPawn())
    {
        coordinates.insert(pawn1);
    }
    Coordinate pawn2 = {position.x - 1, position.y + (isColorWhite ? -1 : 1)};
    if (pawn2.isValidPosition() && isOpponentPieceAt(pawn2, _board) && _board->getPieceAt(pawn2)->isPawn())
    {
        coordinates.insert(pawn2);
    }

    // Check for knights
    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + Knight::MOVE_DIRECTIONS[i].x, position.y + Knight::MOVE_DIRECTIONS[i].y};
        if (next_position.isValidPosition() && isOpponentPieceAt(next_position, _board) && _board->getPieceAt(next_position)->isKnight())
        {
            coordinates.insert(next_position);
        }
    }

    // Check for opponent king
    for (int i = 0; i < 8; i++)
    {
        Coordinate next_position = {position.x + King::MOVE_DIRECTIONS[i].x, position.y + King::MOVE_DIRECTIONS[i].y};
        if (next_position.isValidPosition() && isOpponentPieceAt(next_position, _board) && _board->getPieceAt(next_position)->isKing())
        {
            coordinates.insert(next_position);
        }
    }

    // Check on diagonals(bishop and queen)
    for (int i = 0; i < 4; i++)
    {
        Coordinate direction = Bishop::MOVE_DIRECTIONS[i];
        Coordinate next_position = {direction.x + position.x, direction.y + position.y};
        while (next_position.isValidPosition())
        {
            Piece *pc = _board->getPieceAt(next_position);
            char sy = tolower(pc != nullptr ? pc->getSymbol() : '\0');
            if (isOpponentPieceAt(next_position, _board) && ((sy == 'q') || (sy == 'b')))
            {
                coordinates.insert(next_position);
            }
            else if (pc != nullptr)
            {
                break;
            }

            next_position = {next_position.x + direction.x,
                             next_position.y + direction.y};
        }
    }

    // Check straight lines(rook and queen)
    for (int i = 0; i < 4; i++)
    {
        Coordinate direction = Rook::MOVE_DIRECTIONS[i];
        Coordinate next_position = {direction.x + position.x, direction.y + position.y};
        while (next_position.isValidPosition())
        {
            Piece *pc = _board->getPieceAt(next_position);
            char sy = tolower(pc != nullptr ? pc->getSymbol() : '\0');
            if (isOpponentPieceAt(next_position, _board) && ((sy == 'r') || (sy == 'q')))
            {
                coordinates.insert(next_position);
            }
            else if (pc != nullptr)
            {
                break;
            }

            next_position = {next_position.x + direction.x,
                             next_position.y + direction.y};
        }
    }

    return coordinates;
}