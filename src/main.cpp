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

int main()
{
    Game game;
    game.start();
    
    // char fen[] = "2bqkbnr/Pppppppp/8/8/8/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1";
    // using namespace std;
    // Board board{string(fen)};

    // Piece *piece = board.getPieceAt({0, 4});
    // if (piece == nullptr)
    // {
    //     cout << "No piece found in given position" << endl;
    //     throw "Null";
    // }

    // board.display();
    // board.perfromMove({{1, 0}, {0, 0}, MoveType::Normal});
    // board.display();
    // board.perfromMove({{1, 3}, {0, 3}, MoveType::Normal});
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
