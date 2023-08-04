#pragma once
#include <iostream>

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

    void display()
    {
        if (isValidPosition())
        {
            std::cout << "(" << x << ", " << y << ")\n";
        }
        else
        {
            std::cout << "Not a valid position"
                      << "(" << x << ", " << y << ")" << std::endl;
        }
    }

    void displayChessCoordinate()
    {
        if (isValidPosition())
        {

            std::cout << getChessCoordinate() << std::endl;
        }
        else
        {
            std::cout << "Not a valid position"
                      << "(" << x << ", " << y << ")" << std::endl;
        }
    }

    bool isValidPosition()
    {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    char *getChessCoordinate()
    {
        char *coord = new char[3];
        coord[0] = 'a' + x;
        coord[1] = '1' + 7 - y;
        coord[2] = '\0';
        return coord;
    }

    bool isPromotionSquare(bool isWhitePiece)
    {
        if ((isWhitePiece && x == 0) || (!isWhitePiece && x == 7))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

inline std::ostream &operator<<(std::ostream &o, Coordinate &c)
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

inline bool operator==(Coordinate a, Coordinate b)
{
    return (a.x == b.x && a.y == b.y);
}