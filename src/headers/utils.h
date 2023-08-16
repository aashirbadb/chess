#pragma once
#include "board.h"
#include "move.h"

// C++'s isupper function returns values greater than zero🤷🤷
// We need bool value
inline bool isUppercase(char ch)
{
    return (ch > 'A' && ch < 'Z');
}

inline bool isCapture(Board *_board, Move _move)
{
    Piece *piece = _board->getPieceAt(_move.start);
    return piece != nullptr && piece->isOpponentPieceAt(_move.end, _board);
}

inline bool isPromotion(Board *_board, Move _move)
{
    Piece *piece = _board->getPieceAt(_move.start);

    if (toupper(piece->getSymbol()) == 'P')
    {
        return _move.end.isPromotionSquare(isUppercase(piece->getSymbol()));
    }
    return false;
}

// returns 0 if not castle, 1 if it is castle move and valid, -1 if castle and not valid
inline int isKingSideCastle(Board *_board, Move _move)
{
    Piece *piece = _board->getPieceAt(_move.start);

    if (piece != nullptr && tolower(piece->getSymbol()) == 'k' && _move.start.x == 4 && (_move.end.x == 6 || _move.end.x == 7))
    {
        if (!_board->isPlayerInCheck())
        {
            bool allowed = true;
            std::vector<Move> moves = _board->getAllPlayerMoves(!piece->isWhite());
            int tempy = _board->getIsWhiteTurn() ? 7 : 0;
            // if any of the opponent's pieces attacks the path of the king when castling, it is not allowed
            for (int i = 0; i < moves.size(); i++)
            {
                if (moves[i].end == Coordinate{5, tempy} || moves[i].end == Coordinate{6, tempy})
                {
                    allowed = false;
                    break;
                }
            }

            return allowed ? 1 : -1;
        }

        return -1;
    }

    return 0;
}

// returns 0 if not castle, 1 if it is castle move and valid, -1 if castle and not valid
inline int isQueenSideCastle(Board *_board, Move _move)
{
    Piece *piece = _board->getPieceAt(_move.start);
    if (piece != nullptr && tolower(piece->getSymbol()) == 'k' && _move.start.x == 4 && (_move.end.x == 0 || _move.end.x == 2))
    {
        if (!_board->isPlayerInCheck())
        {
            std::vector<Move> moves = _board->getAllPlayerMoves(!piece->isWhite());
            bool allowed = true;
            int tempy = _board->getIsWhiteTurn() ? 7 : 0;
            for (int i = 0; i < moves.size(); i++)
            {
                if (moves[i].end == Coordinate{2, tempy} || moves[i].end == Coordinate{3, tempy})
                {
                    allowed = false;
                    break;
                }
            }

            return allowed ? 1 : -1;
        }

        return -1;
    }

    return 0;
}

inline bool isPawnTwoMovesAhead(Board *_board, Move _move)
{
    Piece *piece = _board->getPieceAt(_move.start);

    return ((piece != nullptr) && (tolower(piece->getSymbol()) == 'p') && (abs(_move.end.y - _move.start.y) == 2));
}

inline bool isEnpassant(Board *_board, Move _move)
{
    Piece *piece = _board->getPieceAt(_move.start);
    return (piece != nullptr && tolower(piece->getSymbol()) == 'p' && _move.end == _board->getEnpassantTarget());
}