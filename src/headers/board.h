#pragma once
#include <iostream>
#include <vector>
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
const std::string SOME_FEN = std::string("rnb1kbnr/pppppppp/8/3q4/8/8/P3PPPP/R3KBNR w KQkq - 0 1");

enum class GameState
{
    Playing,
    Stalemate,
    WhiteWins,
    BlackWins,
    Draw
};

enum class MoveType
{
    Normal,
    Castle,
    Capture,
    Enpassant,
    Promotion
};

class Board
{
private:
    std::vector<Piece *> allCreatedPiece;
    GameState state;
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

    Piece *promotionPiece;

public:
    Board();
    ~Board();

    Board(std::string fen);
    Board(Board &_board);

    int fromFEN(std::string fen);
    std::string toFEN();

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
    std::vector<Move> getAllPlayerLegalMoves(bool _white);

    bool castlingAvailable(char _type);

    Piece *getPieceAt(Coordinate _coord);
    Coordinate getEnpassantTarget();

    void moveUnchecked(Move _move);
    MoveType performMove(Move _move);
    void setPlayerTurn(bool _turn);

    Piece *createPiece(Coordinate _coord, char type);

    Piece *getWhiteKing();

    Piece *getBlackKing();

    GameState getGameState();

    bool isWaitingForPromotion();
    void promoteTo(char _type);
    Piece *getPromotionPiece();
};
