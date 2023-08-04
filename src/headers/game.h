#pragma
#include <SDL2/SDL.h>
#include "board.h"

#define SQUARE_SIZE 85

class Game
{
protected:
    bool quit;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    Board *board;


public:
    Game();

    void start();
    ~Game();

private:
    void handleEvent();
    void drawBoard();
};