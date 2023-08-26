#pragma once
#include <iostream>
#include "errors.h"

struct Coordinate
{
    int x, y;

    Coordinate()
    {
    }

    Coordinate(int a, int b)
    {
        x = a;
        y = b;
    }

    Coordinate(char a, char b)
    {
        x = a - 'a';
        y = '1' + 7 - b;
    }

    Coordinate(std::string str)
    {
        if (str.length() < 2)
        {
            throw Error("Trying to create coordinate with <2 characters");
        }
        else
        {
            x = str[0] - 'a';
            y = '1' + 7 - str[1];
        }
    }

    bool isValidPosition() const
    {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    char *getChessCoordinate() const
    {
        char *coord = new char[3];
        coord[0] = 'a' + x;
        coord[1] = '1' + 7 - y;
        coord[2] = '\0';
        return coord;
    }

    bool isPromotionSquare(bool isWhitePiece) const
    {
        if ((isWhitePiece && y == 0) || (!isWhitePiece && y == 7))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator<(const Coordinate &other) const
    {
        return (x < other.x) ? true : (y < other.y);
    }

    inline bool operator==(const Coordinate &a) const
    {
        return (a.x == x && a.y == y);
    }

    inline bool operator!=(const Coordinate &a) const
    {
        return !(a == *this);
    }
};

inline std::ostream &operator<<(std::ostream &o, const Coordinate &c)
{
    if (c.isValidPosition())
    {

        o << c.getChessCoordinate();
    }
    else
    {
        o << "Not a valid position"
          << "(" << c.x << ", " << c.y << ")";
    }

    return o;
}
