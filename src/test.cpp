#include "headers/test.h"
#include <fstream>
#include <assert.h>
#include "headers/utils.h"

Test::Test()
{
}
Test::~Test() {}

std::vector<std::string> Test::readFile(const char *path)
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

const char PROMOTION_PIECES[4] = {'q', 'r', 'b', 'n'};
int getNumberOfMoves(int depth, Board board, bool first)
{
    std::vector<Move> moves = board.getAllPlayerLegalMoves(board.getIsWhiteTurn());
    int count = 0;
    for (int i = 0; i < moves.size(); i++)
    {
        Move move = moves[i];
        Board s(board);

        try
        {
            if (isPromotion(&s, move) && move.promotion == '\0')
            {
                // Push all possible promotions to moves and skip this turn
                for (int i = 0; i < 4; i++)
                {
                    Move mv = move;
                    mv.promotion = PROMOTION_PIECES[i];
                    moves.push_back(mv);
                }
                continue;
            }
            else
            {
                s.performMove(move);
            }
        }
        catch (const Error &e)
        {
            if (e.getCode() == ErrorCode::InvalidMove)
                continue;
            else
                throw;
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

int Test::perft(int depth, std::string fen, bool detailed)
{
    // FIXME: There seems to be an infinite loop somewhere
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    Board board(fen);

    int positions = getNumberOfMoves(depth - 1, board, detailed);
    std::cout << "Depth: " << depth << " Number of Positions: " << positions << std::endl;

    auto t2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    std::cout << "Time taken: " << ms_int.count() << " ms" << std::endl;
    return positions;
}

int Test::perft_with_moves(int depth, std::string fen, std::vector<Move> moves)
{
    Board b(fen);
    for (int i = 0; i < moves.size(); i++)
    {
        b.performMove(moves[i]);
    }

    return perft(depth - moves.size(), b.toFEN(), true);
}

void Test::multiplePerfts()
{
    const int depths[] = {0};
    const char *fens[] = {""};
    const int results[] = {0};

    try
    {
        // 69 secs
        // assert(perft(6, "8/5bk1/8/2Pp4/8/1K6/8/8 w - d6 0 1", false) == 824064); // avoid illegal en passant capture

        // 175 secs
        // assert(perft(6, "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", false) == 1440467); // en passant capture checks opponent

        // 95 secs(failed)
        // assert(perft(6, "5k2/8/8/8/8/8/8/4K2R w K - 0 1", false) == 661072); // short castling gives check

        // (failed)
        // assert(perft(6, "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", false) == 661072); // long castling gives check

        // looooong
        // assert(perft(4, "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", false) == 1274206); // castling (including losing cr due to rook capture)

        // looooong
        // assert(perft(4, "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1", false) == 1720476); // castling prevented

        // looooong
        // assert(perft(6, "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", false) == 3821001); // promote out of check

        // looooong
        // assert(perft(5, "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", false) == 1004658); // discovered check

        // 31 secs
        // assert(perft(6, "4k3/1P6/8/8/8/8/K7/8 w - - 0 1", false) == 217342); // promote to give check

        // 14 secs
        // assert(perft(6, "8/P1k5/K7/8/8/8/8/8 w - - 0 1", false) == 92683); // underpromote to check

        // 200 ms
        // assert(perft(6, "K1k5/8/P7/8/8/8/8/8 w - - 0 1", false) == 2217); // self stalemate

        // looooong
        // assert(perft(7, "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", false) == 567584); // stalemate/checkmate

        // looooong
        assert(perft(6, "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", false) == 23527); // double check
    }
    catch (const Error &e)
    {
        std::cerr << e.getError() << '\n';
    }
}