#include "headers/test.h"
#include <fstream>
#include <assert.h>

Test::Test()
{
}
Test::~Test() {}

std::vector<std::string> Test::readFile(char *path)
{
    std::fstream file(path);
    std::vector<std::string> lines;
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
    }

    return lines;
}

void Test::test_fen_parsing()
{
    using namespace std;
    cout << "Testing FEN parsing..." << endl;
    vector<string> lines = readFile("test/lichess_games.txt");
    for (int i = 0; i < lines.size(); i++)
    {
        Board board = Board(lines[i]);
        std::string fen = board.toFEN();
        assert(board.toFEN() == fen);
    }
    cout << "Fen parsing completed successfully: " << lines.size() << " variations" << endl;
}

int getNumberOfMoves(int depth, Board board, bool first)
{
    std::vector<std::vector<Move>> allMoves;
    std::vector<Move> moves = board.getAllPlayerLegalMoves(board.getIsWhiteTurn());

    int count = 0;
    for (Move &move : moves)
    {
        Board s(board);

        s.performMove(move);
        if (s.isWaitingForPromotion())
        {
            s.promoteTo('q', true);
        }
        if (depth == 0)
        {
            count++;
            if (first)
            {
                std::cout << move << " 1" << std::endl;
            }
        }
        else
        {
            int num = 0;
            num = getNumberOfMoves(depth - 1, s, false);

            count += num;
            if (first)
            {

                std::cout << move << " " << num << std::endl;
            }
        }
    }

    return count;
}

void Test::perft(int depth)
{
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    std::cout << "Depth: " << depth << std::endl;

    auto t1 = high_resolution_clock::now();
    Board board(STARTING_FEN);
    std::vector<std::vector<Move>> allMoves;
    std::vector<Move> moves;

    int positions = getNumberOfMoves(depth - 1, board, true);
    std::cout << "Number of Positions: " << positions << std::endl;

    auto t2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    std::cout << "Time taken: " << ms_int.count() << " ms" << std::endl;
}
