#include "headers/stockfish.h"

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
    std::vector<std::string> lines = this->readProcess("uciok", 1000);

    // Get the id of the UCI engine
    for (int i = 0; i < lines.size(); i++)
    {
        if (lines[i].substr(0, 8) == "id name ")
        {
            id = lines[i].substr(8);
        }
    }

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
    // write("go depth 30");
    write("go movetime " + std::to_string(tout));

    std::vector<std::string> data = readProcess("bestmove", tout + 10);
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

std::string StockfishInstance::getID() const
{
    return id;
}

// ========= STOCKFISH =========

StockfishInstance *Stockfish::STOCKFISH_INSTANCE = nullptr;

Stockfish::Stockfish(int lvl, int tout) : Player(false, "Stockfish")
{
    level = std::min(std::max(lvl, 0), 20);
    search_tout = tout;

    // Initialize stockfish only when needed
    if (STOCKFISH_INSTANCE == nullptr)
    {
        STOCKFISH_INSTANCE = new StockfishInstance();
    }
    name = STOCKFISH_INSTANCE->getID();
}

Stockfish::~Stockfish() {}

Move Stockfish::getMove(Board *board)
{
    for (int i = 0; i < 3; i++)
    {
        try
        {
            STOCKFISH_INSTANCE->setLevel(level);
            Move bestmove = STOCKFISH_INSTANCE->getBestMove(board->toFEN(), search_tout);
            bestmove.movetype = board->getMovetype(bestmove);
            return bestmove;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
