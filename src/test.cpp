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