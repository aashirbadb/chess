#define SDL_MAIN_HANDLED 1
#include "headers/game.h"
#include "headers/texture.h"
#include "headers/ChessGameScene.h"
#include "headers/board.h"
#include "headers/test.h"

int main(int argc, char **argv)
{
    try
    {
        Test::multiplePerfts();
    }
    catch (const Error &e)
    {
        std::cerr << e.getError() << '\n';
    }

    // try
    // {
    //     Game game;
    //     game.start();
    //     Texture::clearTextures();
    // }
    // catch (const Error &e)
    // {
    //     std::cerr << e.getError() << std::endl;
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << e.what() << '\n';
    // }

    return 0;
}
