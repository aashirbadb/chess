#pragma once

struct Coordinate
{
    int x, y;

    bool isValidPosition()
    {

        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }
};