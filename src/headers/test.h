#pragma once
#include <iostream>
#include <vector>
#include "board.h"
#include <chrono>
#include "move.h"

class Test
{
public:
    Test();
    ~Test();

    static std::vector<std::string> readFile(const char *path);

    static void test_fen_parsing();

    static int perft(int depth, std::string fen = STARTING_FEN, bool detailed = true);
    static int perft_with_moves(int depth, std::string fen = STARTING_FEN, std::vector<Move> moves = std::vector<Move>());

    static void multiplePerfts();
};