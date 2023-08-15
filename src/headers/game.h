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
#include "sound.h"

#define SQUARE_SIZE 85
#define BOARD_SIZE (SQUARE_SIZE * 8)
#define WINDOW_HEIGHT BOARD_SIZE
#define LEFT_OFFSET 200
#define RIGHT_OFFSET 200
#define WINDOW_WIDTH (BOARD_SIZE + LEFT_OFFSET + RIGHT_OFFSET)

#define FPS 60

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

struct WindowSize
{
    int height, width;
    int topOffset, bottomOffset;
    int leftOffset, rightOffset;
    int tileSize, boardSize;
};

inline bool operator==(WindowSize a, WindowSize b)
{
    return (a.height == b.height && a.width == b.width);
}

inline bool operator!=(WindowSize a, WindowSize b)
{
    return !(a.height == b.height && a.width == b.width);
}

class Game
{
    bool quit;
    WindowSize sizes;
    bool muted;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    SoundManager *sound;
    std::stack<GameScene *> scenes;

public:
    Game();
    ~Game();
    void start();

    SDL_Renderer *getRenderer();
    void quitGame();
    void calculateWindowSize();
    WindowSize getWindowSize() const;

    void playSound(Sound _sound);

    void pushScene(GameScene *sc);
    GameScene *popScene();
    GameScene *currentScene();
};