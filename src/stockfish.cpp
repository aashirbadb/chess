#include "headers/stockfish.h"
#include <iostream>

// ========= STOCKFISH INSTANCE =========
StockfishInstance::StockfishInstance()
{
    std::cerr
        << "Initializing Stockfish" << std::endl;

#ifdef _WIN64
    this->initProcess("assets/stockfish-windows.exe");
#else
    this->initProcess("assets/stockfish-linux");
#endif

    write("uci");
    this->readProcess("uciok", 1000);

    write("ucinewgame");

    if (!isReady())
    {
        throw std::runtime_error("Error while iniitalizing stockfish");
    }

    std::cerr << "Stockfish initialized" << std::endl;
}

StockfishInstance::~StockfishInstance()
{
    killProcess();
}

void StockfishInstance::write(std::string str)
{
    writeProcess(str + "\n");
}

bool StockfishInstance::isReady()
{
    write("isready");
    readProcess("readyok", 50);

    return !timeout();
}

Move StockfishInstance::getBestMove(std::string fen, int tout = 1000)
{
    write("position fen " + fen);
    write("go depth 30");

    std::vector<std::string> data = readProcess("bestmove", tout);
    if (timeout())
    {
        // stop searching if timeout was reached and return get the bestmove from there
        write("stop");
        data = readProcess("bestmove", 50);
    }

    std::string lastLine = data.back();
    if (lastLine.substr(0, 9) == "bestmove ")
    {
        std::string sub = lastLine.substr(9);
        std::string move;

        for (int i = 0; i < sub.size() && sub[i] != ' '; i++)
        {
            move += sub[i];
        }

        // std::cout << lastLine << std::endl;

        Move mv(move);
        return mv;
    }
    else
    {
        throw Error("Could not find best move");
    }
}

// To set an option:
// setoption name <name> value <value>

// Limit to specific Elo
// write("setoption name UCI_LimitStrength value true");
// write("setoption name UCI_Elo value "); // type spin default 1320 min 1320 max 3190
void StockfishInstance::setOption(std::string option, std::string value)
{
    write("setoption name " + option + " value " + value);
}
void StockfishInstance::setLevel(int lvl)
{
    write("setoption name Skill Level value " + std::to_string(lvl)); // Skill Level (0-20)
}

// ========= STOCKFISH =========

StockfishInstance Stockfish::STOCKFISH_INSTANCE = StockfishInstance();

Stockfish::Stockfish(int lvl, int tout) : Player(false, "Stockfish")
{
    level = std::min(std::max(lvl, 0), 20);
    search_tout = tout;
}

Stockfish::~Stockfish() {}

Move Stockfish::getMove(Board *board)
{
    for (int i = 0; i < 3; i++)
    {
        try
        {
            STOCKFISH_INSTANCE.setLevel(level);
            return STOCKFISH_INSTANCE.getBestMove(board->toFEN(), search_tout);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
