#pragma once

enum PieceType
{
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King
};

class Piece
{
    int x, y;
    bool iswhite;
    PieceType type;

public:
    Piece();

    Piece(int a, int b, char c);

    int moveTo(int a, int b);

    int getValidMoves();

    int canMoveTo();

    char getSymbol();
};