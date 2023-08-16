#define SDL_MAIN_HANDLED 1
#include <iostream>
#include "headers/board.h"
#include <vector>
#include "headers/test.h"
#include "headers/game.h"
#include "headers/texture.h"
#include "headers/stockfish.h"

void show_valid_positions_at(Coordinate coord, Board &board)
{
    Piece *piece = board.getPieceAt(coord);
    if (piece == nullptr)
        return;
    std::vector<Move>
        valid = piece->getAllMoves(board);
    if (valid.size() > 0)
    {

        std::cerr << "Showing all valid moves for " << piece->getSymbol() << " at " << piece->getPosition().getChessCoordinate() << std::endl;
        for (int i = 0; i < valid.size(); i++)
        {
            std::cerr << valid[i] << '\t';
        }
        std::cerr << '\n';
    }
    else
    {
        std::cerr << "No valid positions for " << piece->getSymbol() << " at " << piece->getPosition().getChessCoordinate() << std::endl;
    }
    std::cerr << std::endl;
}

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        Game game;
        game.start();
    }
    else
    {
        using namespace std;
        Board board{SOME_FEN};
        board.performMove(Move{Coordinate('d', '8'), Coordinate('a', '5')});
        board.performMove(Move{Coordinate('d', '2'), Coordinate('d', '4')});
        board.performMove(Move{Coordinate('a', '5'), Coordinate('e', '1')});
        board.display();
    }

    Texture::clearTextures();
    // Stockfish stf;
    // stf.getBestMove(STARTING_FEN);

    // Board b;
    // b.display();
    // std::cout << STARTING_FEN << std::endl
    //           << b.toFEN() << std::endl;

    return 0;
}
