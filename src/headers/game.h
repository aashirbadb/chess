#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stack>
#include "board.h"
#include "piece.h"
#include "coordinate.h"
#include "move.h"
#include "gameScene.h"
#include "texture.h"
#include <SDL2/SDL_blendmode.h>

#define SQUARE_SIZE 85
#define BOARD_SIZE SQUARE_SIZE * 8
#define WINDOW_HEIGHT BOARD_SIZE
#define WINDOW_WIDTH BOARD_SIZE
namespace color
{
    const SDL_Color WHITE = {255, 255, 255, SDL_ALPHA_OPAQUE};
    const SDL_Color BLACK = {0, 0, 0, SDL_ALPHA_OPAQUE};
    const SDL_Color RED = {255, 0, 0, SDL_ALPHA_OPAQUE};
    const SDL_Color GREEN = {0, 255, 0, SDL_ALPHA_OPAQUE};
    const SDL_Color BLUE = {0, 0, 255, SDL_ALPHA_OPAQUE};

    const SDL_Color WHITE_TILE = WHITE;
    const SDL_Color BLACK_TILE = BLACK;
    const SDL_Color CHECK_TILE = RED;
    const SDL_Color SELECTED_TILE = GREEN;
    const SDL_Color MOVE_TILE = GREEN;
}

class GameScene;

class Game
{
    bool quit;
    bool render_requested;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    std::stack<GameScene *> scenes;

public:
    Game();
    ~Game();
    void start();

    SDL_Renderer *getRenderer();
    void quitGame();
    void requestRender();

    void pushScene(GameScene *sc);
    GameScene *popScene();
    GameScene *currentScene();
};