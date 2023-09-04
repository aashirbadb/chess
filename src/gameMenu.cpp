#include "headers/gameMenu.h"
#include "headers/texture.h"
#include "headers/gameScene.h"
#include "headers/ChessGameScene.h"
#include "headers/game.h"

GameMenu::GameMenu(Game *g) : GameScene(g)
{
    singleplayerbutton = {0, 0, 0, 0};
    multiplayerbutton = {0, 0, 0, 0};
    muteButton = {0, 0, 0, 0};

    loadTextures();
}

GameMenu::~GameMenu()
{
}

void GameMenu::render()
{
    WindowSize wsize = game->getWindowSize();

    SDL_Rect bg = {0, 0, wsize.width, wsize.height};
    backgroundTexture.draw(NULL, &bg);

    int padding = 20;
    singleplayerbutton = {
        wsize.width / 2 - singleplayerTexture.getWidth() / 2 - padding,
        wsize.height / 2 - singleplayerTexture.getHeight() - padding,
        singleplayerTexture.getWidth() + padding,
        singleplayerTexture.getHeight() + padding,
    };

    multiplayerbutton = {
        wsize.width / 2 - multiplayerTexture.getWidth() / 2 - padding,
        wsize.height / 2 + multiplayerTexture.getHeight() - padding,
        multiplayerTexture.getWidth() + padding,
        multiplayerTexture.getHeight() + padding,
    };

    SDL_SetRenderDrawColor(game->getRenderer(), 255, 255, 255, 128);
    SDL_RenderFillRect(game->getRenderer(), &singleplayerbutton);
    SDL_RenderFillRect(game->getRenderer(), &multiplayerbutton);

    singleplayerTexture.draw(singleplayerbutton.x + padding / 2, singleplayerbutton.y + padding / 2);

    multiplayerTexture.draw(multiplayerbutton.x + padding / 2, multiplayerbutton.y + padding / 2);

    muteButton = {wsize.width - 60, wsize.height - 60, 50, 50};
    if (game->getMuted())
    {
        muteTexture.draw(NULL, &muteButton);
    }
    else
    {
        soundTexture.draw(NULL, &muteButton);
    }
}

void GameMenu::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (hasClickedInsideButton(x, y, singleplayerbutton))
        {
            bool whitehuman = rand() % 2 == 0;
            game->pushScene(new ChessGame(game, whitehuman, !whitehuman));
            game->playSound(Sound::ButtonClick);
        }
        else if (hasClickedInsideButton(x, y, multiplayerbutton))
        {
            game->pushScene(new ChessGame(game, true, true));
            game->playSound(Sound::ButtonClick);
        }
        else if (hasClickedInsideButton(x, y, muteButton))
        {
            game->setMuted(!game->getMuted());
            game->playSound(Sound::ButtonClick);
        }
    }
}

void GameMenu::update() {}

void GameMenu::handleResize()
{
    loadTextures();
}

void GameMenu::loadTextures()
{
    WindowSize wsize = game->getWindowSize();

    singleplayerTexture.setRenderer(game->getRenderer());
    singleplayerTexture.loadString("Singleplayer", 48, color::BLACK);

    multiplayerTexture.setRenderer(game->getRenderer());
    multiplayerTexture.loadString("Two Players", 48, color::BLACK);

    if (backgroundTexture.getTexture() == nullptr)
    {
        backgroundTexture.setRenderer(game->getRenderer());
        backgroundTexture.loadImage("assets/background.jpg");
    }

    soundTexture.setRenderer(game->getRenderer());
    soundTexture.loadImage("assets/sound.png");

    muteTexture.setRenderer(game->getRenderer());
    muteTexture.loadImage("assets/mute.png");
}