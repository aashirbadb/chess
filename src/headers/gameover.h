#pragma once
#include "gameScene.h"
#include "game.h"
#include <SDL2/SDL.h>

class GameOver : public GameScene
{
    GameState state;
    SDL_Rect menubutton;

public:
    GameOver(Game *g, GameState _state);

    void render() override;
    void handleEvent(SDL_Event &e) override;
};