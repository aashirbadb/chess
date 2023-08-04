#define SDL_MAIN_HANDLED 1
#include <iostream>
#include "headers/board.h"
#include <vector>
#include "headers/test.h"
#include "headers/game.h"

void show_valid_positions_at(Coordinate coord, Board &board)
{
    Piece *piece = board.getPieceAt(coord);
    if (piece == nullptr)
        return;
    std::vector<Move>
        valid = piece->getAllMoves(board);
    if (valid.size() > 0)
    {

        std::cout << "Showing all valid moves for " << piece->getSymbol() << " at " << piece->getPosition().getChessCoordinate() << std::endl;
        for (int i = 0; i < valid.size(); i++)
        {
            std::cout << valid[i] << '\t';
        }
        std::cout << '\n';
    }
    else
    {
        std::cout << "No valid positions for " << piece->getSymbol() << " at " << piece->getPosition().getChessCoordinate() << std::endl;
    }
    std::cout << std::endl;
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
        board.display();
        board.performMove(Move{Coordinate('d', '8'), Coordinate('a', '5'), MoveType::Normal});
        board.performMove(Move{Coordinate('d', '2'), Coordinate('d', '4'), MoveType::Normal});
        board.performMove(Move{Coordinate('a', '5'), Coordinate('e', '1'), MoveType::Normal});
        board.display();

        // while(true){
        //     board.getPieceAt({4,0})->getLegalMoves(board);
        // }
        // show_valid_positions_at({6, 0}, board);
    }

    // Piece *piece = board.getPieceAt({0, 1});
    // if (piece == nullptr)
    // {
    //     cout << "No piece found in given position" << endl;
    //     throw "Null";
    // }

    // Coordinate pos = piece->getPosition();
    // cout << pos << endl;

    // board.display();
    // board.performMove({{1, 0}, {0, 0}, MoveType::Normal});
    // board.display();
    // board.performMove({{1, 3}, {0, 3}, MoveType::Normal});
    // board.display();

    // std::vector<Move> legal = board.getAllPlayerMoves(true);
    // if (legal.size() == 0)
    // {
    //     cout << "No legal moves found for this position" << endl;
    // }
    // else
    // {
    //     cout << "Legal moves for " << (board.getIsWhiteTurn() ? "white" : "black") << ": ";
    //     for (int i = 0; i < legal.size(); i++)
    //     {
    //         cout << legal[i] << endl;
    //     }
    // }

    return 0;
}
