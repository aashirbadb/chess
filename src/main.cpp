#include <stdio.h>
#include "headers/board.h"
#include <vector>

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
            valid[i].display();
            std::cout << '\t';
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
    Board board = Board(SOME_FEN);
    board.display();

    // for (int i = 0; i < 8; i++)
    // {
    //     for (int j = 0; j < 8; j++)
    //     {
    //         show_valid_positions_at({i, j},board);
    //     }
    // }

    show_valid_positions_at({4, 4}, board);

    return 0;
}
