#pragma once
#include "communication.hpp"
#include "move.h"
#include <string>
#include "errors.h"
#include "player.h"
#include "board.h"

class StockfishInstance : private Communication::Process
{
public:
    StockfishInstance();
    ~StockfishInstance() override;

    Move getBestMove(std::string fen, int tout);
    bool isReady();
    void setOption(std::string option, std::string value);
    void setLevel(int lvl);

protected:
    void write(std::string str);
};

// https://disservin.github.io/stockfish-docs/pages/Commands.html
class Stockfish : public Player
{
    int level, search_tout;
    static StockfishInstance *STOCKFISH_INSTANCE;

public:
    Stockfish(int lvl = 10, int tout = 200);
    ~Stockfish();

    Move getMove(Board *board) override;
};