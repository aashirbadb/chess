#pragma once
#include "coordinate.h"
#include <iostream>

enum class MoveType
{
    Normal,
    Capture,
    Promotion,
    PawnFirstMove,
    Enpassant,
    KingsideCastle,
    QueensideCastle
};

struct Move
{
    Coordinate start, end;
    MoveType type;
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