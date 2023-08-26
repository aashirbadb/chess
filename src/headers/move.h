#pragma once
#include "coordinate.h"
#include "errors.h"
#include <iostream>

struct Move
{
    Coordinate start, end;
    char promotion;

    Move()
    {
        start = {-1, -1};
        end = {-1, -1};
        promotion = 0;
    }

    Move(Coordinate x, Coordinate y)
    {
        start = x;
        end = y;
        promotion = 0;
    }

    Move(std::string str)
    {
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

    Move(char *chars)
    {
        std::string str(chars);
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

    inline operator std::string()
    {
        std::string ret(start.getChessCoordinate());
        ret.append(end.getChessCoordinate());
        ret.push_back(promotion);
        return ret;
    }

    inline bool operator==(const Move &a) const
    {
        return a.start == start && a.end == end;
    }

    bool operator<(const Move &other) const
    {
        return (start < other.start) ? true : (end < other.end);
    }
};

inline std::ostream &operator<<(std::ostream &o, const Move &move)
{
    o << move.start << move.end << move.promotion;
    return o;
}
