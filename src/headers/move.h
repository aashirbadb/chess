#pragma once
#include "coordinate.h"
#include "errors.h"
#include <iostream>
#include "piece.h"

class Piece;

struct Move
{
    Coordinate start, end;
    char promotion;
    Piece *capturedPiece; // used to undo last move

    Move()
    {
        start = {-1, -1};
        end = {-1, -1};
        promotion = 0;
        capturedPiece = nullptr;
    }

    Move(Coordinate x, Coordinate y)
    {
        start = x;
        end = y;
        promotion = 0;
        capturedPiece = nullptr;
    }

    Move(std::string str)
    {
        capturedPiece = nullptr;
        if (str.length() < 4)
        {
            throw Error("Trying to create a move with <4 characters");
        }
        else
        {
            start = str.substr(0, 2);
            end = str.substr(2, 2);
            promotion = 0;
            if (str.length() == 5)
                promotion = str.at(4);
        }
    }
};

inline std::ostream &operator<<(std::ostream &o, Move &m)
{
    o << m.start << m.end;
    return o;
}

inline bool operator==(Move &a, Move &b)
{
    return a.start == b.start && a.end == b.end;
}
