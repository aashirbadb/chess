#pragma once
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
    virtual void handleEvent(SDL_Event &e) {}
    virtual void handleRightMouse(SDL_Event &e) {}
    virtual void handleLeftMouse(SDL_Event &e) {}
    virtual void handleKeyPress(SDL_Event &e) {}

    virtual void render() = 0;
    virtual void update() = 0;
    virtual void handleResize() {}

    static inline bool hasClickedInsideButton(int x, int y, SDL_Rect rect)
    {
        return (x > rect.x && x < rect.x + rect.w) && (y > rect.y && y < rect.y + rect.h);
    }
};