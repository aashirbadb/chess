#pragma once
#include "communication.hpp"
#include "move.h"
#include <string>
#include "errors.h"
#include "player.h"
#include "board.h"

// https://disservin.github.io/stockfish-docs/pages/Commands.html
class Stockfish : public Player, private Communication::Process
{
    int level;
    int search_tout;

public:
    Stockfish(int lvl, int tout = 200);
    ~Stockfish() override;

    Move getBestMove(std::string fen);
    Move getMove(Board *board) override;

protected:
    bool isReady();
    void write(std::string str);
};