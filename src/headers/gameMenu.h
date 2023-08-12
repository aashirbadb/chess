#pragma once
#include "gameScene.h"

class GameMenu : public GameScene
{
    SDL_Rect startbutton;

public:
    GameMenu(Game *g);

    void render() override;
    void handleEvent(SDL_Event &e) override;
};