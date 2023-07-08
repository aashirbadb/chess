#include <stdio.h>
#include "headers/board.h"
#include <vector>
#include "headers/test.h"

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
    char fen[] = "rnbqkbnr/ppp1pppp/8/1B1p4/8/4P3/PPPP1PPP/RNBQK1NR b KQkq - 1 2";
    using namespace std;
    Board board = Board(string(fen));
    board.display();

    Piece *piece = board.getPieceAt({0, 2});
    cout << "Checking at: " << piece->getPosition().getChessCoordinate() << endl;
    if (piece == nullptr)
    {
        throw "Null";
    }
    std::vector<Move> legal = piece->getLegalMoves(board);
    if (legal.size() == 0)
    {
        cout << "No legal moves found for this position" << endl;
    }
    else
    {
        cout << "Legal moves for " << (board.getIsWhiteTurn() ? "white" : "black") << ": ";
        for (int i = 0; i < legal.size(); i++)
        {
            legal[i].display();
            cout << endl;
        }
    }

    return 0;
}
