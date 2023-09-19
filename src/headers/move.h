#pragma once
#include "coordinate.h"
#include "errors.h"
#include <iostream>

#define MOVETYPE_NONE (0)
#define MOVETYPE_CAPTURE (1)
#define MOVETYPE_PAWN_TWOMOVES (1 << 1)
#define MOVETYPE_KINGSIDE_CASTLE (1 << 2)
#define MOVETYPE_QUEENSIDE_CASTLE (1 << 3)
#define MOVETYPE_PROMOTION (1 << 4)
#define MOVETYPE_ENPASSANT (1 << 5)
#define MOVETYPE_UNKNOWN (1 << 6)
struct Move
{
    Coordinate start, end;
    char promotion;
    int movetype;

    Move()
    {
        start = {-1, -1};
        end = {-1, -1};
        promotion = 0;
        movetype = MOVETYPE_UNKNOWN;
    }

    Move(Coordinate x, Coordinate y, int type)
    {
        start = x;
        end = y;
        promotion = 0;
        movetype = type;
    }

    Move(std::string str)
    {
        if (str.length() < 4)
        {
            throw Error("Trying to create a move with <4 characters");
        }
        else
        {
            start = str.substr(0, 2);
            end = str.substr(2, 2);
            promotion = 0;
            movetype = MOVETYPE_UNKNOWN;
            if (str.length() == 5)
                promotion = str.at(4);
        }
    }

    Move(char *chars)
    {
        std::string str(chars);
        if (str.length() < 4)
        {
            throw Error("Trying to create a move with <4 characters");
        }
        else
        {
            start = str.substr(0, 2);
            end = str.substr(2, 2);
            promotion = 0;
            movetype = MOVETYPE_UNKNOWN;
            if (str.length() == 5)
                promotion = str.at(4);
        }
    }

    inline bool isCapture()
    {
        return movetype & MOVETYPE_CAPTURE;
    }

    inline bool isPromotion()
    {
        return movetype & MOVETYPE_PROMOTION;
    }

    inline bool isKingSideCastle()
    {
        return movetype & MOVETYPE_KINGSIDE_CASTLE;
    }

    inline bool isQueenSideCastle()
    {
        return movetype & MOVETYPE_QUEENSIDE_CASTLE;
    }

    inline bool isCastle()
    {
        return isQueenSideCastle() || isKingSideCastle();
    }

    inline bool isPawnTwoMovesAhead()
    {
        return movetype & MOVETYPE_PAWN_TWOMOVES;
    }

    inline bool isEnpassant()
    {
        return movetype & MOVETYPE_ENPASSANT;
    }

    inline bool isUnknown()
    {
        return movetype & MOVETYPE_UNKNOWN;
    }

    inline operator std::string()
    {
        std::string ret(start.getChessCoordinate());
        ret.append(end.getChessCoordinate());
        ret.push_back(promotion);
        return ret;
    }

    inline bool operator==(const Move &a) const
    {
        return a.start == start && a.end == end;
    }

    bool operator<(const Move &other) const
    {
        return (start < other.start) ? true : (end < other.end);
    }
};

inline std::ostream &operator<<(std::ostream &o, const Move &move)
{
    o << move.start << move.end << move.promotion;
    return o;
}
