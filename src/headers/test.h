#pragma once
#include <iostream>
#include <vector>
#include "board.h"

class Test
{
public:
    Test();
    ~Test();

    std::vector<std::string> readFile(char *path);

    void test_fen_parsing();
};