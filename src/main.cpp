#define SDL_MAIN_HANDLED 1
#include "headers/game.h"
#include "headers/texture.h"
#include "headers/ChessGameScene.h"

int main(int argc, char **argv)
{
    try
    {
        Game game;
        game.pushScene(new ChessGame(&game));
        game.start();
        Texture::clearTextures();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
