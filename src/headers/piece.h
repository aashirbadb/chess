#pragma once
#include "move.h"
#include "coordinate.h"
#include <vector>

struct Move;

class Board;
class Piece
{
protected:
    Coordinate position;
    bool isColorWhite;

public:
    Piece(Coordinate _pos, bool _isWhite);
    ~Piece();

    Coordinate getPosition();
    bool isWhite();

    // Get all moves, including invalid(cannot move due to check, blocking pieces) ones
    virtual std::vector<Move> getAllMoves(Board &_board) = 0;

    // Get the symbol of the piece(to show in the console)
    virtual char getSymbol() = 0;

    // returns true if there is opponent piece in given coordinate
    bool isOpponentPieceAt(Coordinate _coord, Board *_board);

    // returns true if there is onw piece in given coordinate
    bool isOwnPieceAt(Coordinate _coord, Board *_board);

    std::vector<Move> getLegalMoves(Board &_board);

    void updateCoordinate(Coordinate _position);
};
