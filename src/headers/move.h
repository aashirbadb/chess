#pragma once
#include "coordinate.h"
#include <iostream>

namespace moveType
{
    enum MoveType
    {
        Normal,
        Capture,
        Promotion,
        PawnFirstMove,
        Enpassant,
        KingsideCastle,
        QueensideCastle
    };
}

struct Move
{
    Coordinate start, end;
    moveType::MoveType type;
};

inline std::ostream &operator<<(std::ostream &o, Move &m)
{
    o << m.start << m.end;
    return o;
}
