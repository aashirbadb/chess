#pragma once
#include "coordinate.h"
#include <iostream>

struct Move{
    Coordinate start, end;

    void display(){
        std::cout << start.getChessCoordinate() << end.getChessCoordinate();
    }
};