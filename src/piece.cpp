
#include "headers/piece.h"
#include <iostream>

bool isLowercase(char ch)
{
    return ch >= 'a' && ch <= 'z';
}

char toLowercase(char ch)
{
    if (!isLowercase(ch))
    {
        return ch + ('a' - 'A');
    }
    else
    {
        return ch;
    }
}

Piece::Piece()
{
    x = 0;
    y = 0;
    iswhite = true;
    type = Rook;
}

Piece::Piece(int a, int b, char c)
{
    using namespace std;
    // cout << "Creating piece " << c << " at (" << a << ',' << b << ')' <<endl;
    x = a;
    y = b;
    iswhite = !isLowercase(c); // lowercase represents white pieces
    switch (toLowercase(c))
    {
    case 'p':
        type = Pawn;
        break;
    case 'r':
        type = Rook;
        break;
    case 'n':
        type = Knight;
        break;
    case 'b':
        type = Bishop;
        break;
    case 'q':
        type = Queen;
        break;
    case 'k':
        type = King;
        break;

    default:
        throw; // TODO: Throw proper error
    }

    // cout << "piece " << getSymbol() << " at (" << a << ',' << b << ')' <<endl;
}

int Piece::moveTo(int a, int b)
{
    x = a;
    y = b;
    return 0;
}

int Piece::getValidMoves()
{
    return 0;
}

int Piece::canMoveTo()
{
    return 0;
}

char Piece::getSymbol()
{
    char symbol;
    switch (type)
    {
    case Pawn:
        symbol = 'P';
        break;
    case Rook:
        symbol = 'R';
        break;
    case Knight:
        symbol = 'N';
        break;
    case Bishop:
        symbol = 'B';
        break;
    case Queen:
        symbol = 'Q';
        break;
    case King:
        symbol = 'K';
        break;
    default:
        symbol = 'O';
        break;
        // printf("ERROR: %d\n", symbol);
        // throw; // TODO:
        // break;
    }

    if (!iswhite)
    {
        return toLowercase(symbol);
    }
    else
    {
        return symbol;
    }
}
