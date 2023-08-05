#pragma once
#include <iostream>
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include "coordinate.h"
#include "piece.h"

class Piece;

// <pieces(rows are separated by /)> <turn> <castling availibility> <En passant target square> <Halfmove clock> <Fullmove number>
const std::string EMPTY_FEN = std::string("8/8/8/8/8/8/8/8 w KQkq - 0 1");
const std::string STARTING_FEN = std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
const std::string SOME_FEN = std::string("rnbqkbnr/pp2pppp/8/2p5/4P3/5N2/PPPp1PPP/RNBQKB1R w KQkq - 1 2");

enum class GameState
{
    Playing,
    Stalemate,
    WhiteWins,
    BlackWins,
    Draw
};

class Board
{
private:
    std::vector<Piece *> allCreatedPiece;
    GameState state;

protected:
    Piece *pieces[8][8];
    bool isWhiteTurn;
    bool canCastle[4];
    Coordinate enPassantTarget;
    // The number of halfmoves since the last capture or pawn advance, used for the fifty-move rule.
    int halfMoveClock;
    // The number of the full moves. It starts at 1 and is incremented after Black's move.
    int fullMoveClock;
    Piece *whiteKing;
    Piece *blackKing;

public:
    Board();
    ~Board();

    Board(std::string fen);
    Board(Board &_board);

    int fromFEN(std::string fen);

    void display();
    void display_meta();

    bool getBoardColorAt(int _x, int _y);

    bool getIsWhiteTurn();

    std::vector<Piece *> getWhitePieces();
    std::vector<Piece *> getBlackPieces();

    bool isWhiteInCheck();
    bool isBlackInCheck();
    bool isOpponentInCheck();
    bool isPlayerInCheck();

    std::vector<Move> getAllPlayerMoves(bool _white);

    bool castlingAvailable(MoveType _type, bool _isWhitePiece);

    Piece *getPieceAt(Coordinate _coord);
    Coordinate getEnpassantTarget();
    void setEnpassantTarget(Coordinate _coord);

    void moveUnchecked(Move _move);
    void performMove(Move _move);
    void setPlayerTurn(bool _turn);

    Piece *createPiece(Coordinate _coord, char type);

    Piece *getWhiteKing();

    Piece *getBlackKing();

    GameState getGameState();
};
