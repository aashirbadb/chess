#pragma once
#include "move.h"
#include "board.h"
#include <string>

class Player
{
protected:
    bool human;
    std::string name;

public:
    Player(bool h, std::string _name) : human(h), name(_name) {}
    virtual Move getMove(Board *board) = 0;
    bool isHuman() { return human; }
    std::string getName() { return name; }
};

class Human : public Player
{
public:
    Human(std::string _name) : Player(true, _name) {}
    // This is handled by the UI
    Move getMove(Board *board) override
    {
        return {{-1, -1}, {-1, -1}, MOVETYPE_UNKNOWN};
    }
};