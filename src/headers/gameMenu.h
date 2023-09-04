#pragma once
#include <SDL2/SDL.h>

#include "gameScene.h"
#include "texture.h"

class GameMenu : public GameScene
{
    SDL_Rect startbutton;

    Texture singleplayerTexture;
    Texture multiplayerTexture;
    Texture backgroundTexture;
    Texture muteTexture;
    Texture soundTexture;

    SDL_Rect muteButton;
    SDL_Rect singleplayerbutton;
    SDL_Rect multiplayerbutton;

public:
    GameMenu(Game *g);
    ~GameMenu();

    void render() override;
    void handleEvent(SDL_Event &e) override;
    void update() override;
    void handleResize() override;

protected:
    void loadTextures();
};