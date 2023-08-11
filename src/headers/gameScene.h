#pragma once
#include "game.h"
#include "texture.h"
#include <SDL2/SDL.h>

class Game;

// Base class for all scenes(screen) present in the game.
class GameScene
{
protected:
    Game *game;

public:
    GameScene(Game *g) : game(g) {}
    virtual ~GameScene() {}
    virtual void handleEvent(SDL_Event &e) = 0;
    virtual void render() = 0;
};