#pragma once
#include <iostream>

struct Coordinate
{
    int x, y;

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
        coord[0] = 'a' + y;
        coord[1] = '1' + 7 - x;
        coord[2] = '\0';
        return coord;
    }
};
