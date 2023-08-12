#pragma once
#include "coordinate.h"
#include <iostream>

struct Move
{
    Coordinate start, end;
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