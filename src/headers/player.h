#pragma once
#include "move.h"
#include "board.h"

class Player
{
    bool human;

public:
    Player(bool h) : human(h) {}
    virtual Move getMove(Board *board) = 0;
    bool isHuman() { return human; }
};

class Human : public Player
{
public:
    Human() : Player(true) {}
    // This is handled by the UI
    Move getMove(Board *board) override
    {
        return {{-1, -1}, {-1, -1}};
    }
};