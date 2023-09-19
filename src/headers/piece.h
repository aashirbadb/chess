#pragma once
#include <vector>

struct Move;
class Board;

#include "coordinate.h"
#include "move.h"

class Piece
{
protected:
    Coordinate position;
    bool isColorWhite;

public:
    Piece(Coordinate _pos, bool _isWhite);
    ~Piece();

    // Get all moves, including invalid(cannot move due to check, blocking pieces) ones
    virtual std::vector<Move> getAllMoves(Board *_board) = 0;

    // Get the symbol of the piece(to show in the console)
    virtual char getSymbol() = 0;

    // returns true if there is opponent piece in given coordinate
    bool isOpponentPieceAt(Coordinate _coord, Board *_board);

    // returns true if there is onw piece in given coordinate
    bool isOwnPieceAt(Coordinate _coord, Board *_board);

    std::vector<Move> getLegalMoves(Board *_board);

    inline Coordinate getPosition() { return position; }
    inline bool isWhite() { return isColorWhite; }
    inline void updateCoordinate(Coordinate _position) { position = _position; }

    inline bool isKing() { return getSymbol() == 'K' || getSymbol() == 'k'; }
    inline bool isQueen() { return getSymbol() == 'Q' || getSymbol() == 'q'; }
    inline bool isRook() { return getSymbol() == 'R' || getSymbol() == 'r'; }
    inline bool isBishop() { return getSymbol() == 'B' || getSymbol() == 'b'; }
    inline bool isKnight() { return getSymbol() == 'N' || getSymbol() == 'n'; }
    inline bool isPawn() { return getSymbol() == 'P' || getSymbol() == 'p'; }
};
