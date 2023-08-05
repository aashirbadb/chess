#pragma
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "board.h"
#include "piece.h"
#include "coordinate.h"
#include "move.h"

#define SQUARE_SIZE 85

class Game
{

protected:
    bool quit;
    bool render_requested;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    Board *board;
    Piece *selected_piece;

public:
    Game();

    void start();
    ~Game();

private:
    static char rowlabel[9], columnlabel[9];

    void handleEvent();
    void handleMouseLeftClick();
    void drawBoard();
};