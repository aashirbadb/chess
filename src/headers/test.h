#pragma once
#include <iostream>
#include <vector>
#include "board.h"
#include <chrono>

class Test
{
public:
    Test();
    ~Test();

    std::vector<std::string> readFile(char *path);

    void test_fen_parsing();

    void perft(int depth);
};