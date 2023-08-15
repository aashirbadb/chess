#include "headers/gameover.h"
#include "headers/texture.h"
#include "headers/gameMenu.h"

GameOver::GameOver(Game *g, GameState _state) : GameScene(g)
{
    state = _state;
    menubutton = {0, 0, 0, 0};
}

void GameOver::render()
{
    WindowSize wsize = game->getWindowSize();
    SDL_Rect window_rect = {0, 0, wsize.width, wsize.height};

    SDL_Color clr = color::BLACK;
    SDL_SetRenderDrawColor(game->getRenderer(), clr.r, clr.g, clr.b, clr.a);
    SDL_RenderFillRect(game->getRenderer(), &window_rect);
    Texture text(game->getRenderer());
    switch (state)
    {
    case GameState::WhiteWins:
        text.loadString("White Wins!", 50, color::WHITE);
        break;

    case GameState::BlackWins:
        text.loadString("Black Wins!", 50, color::WHITE);
        break;
    case GameState::Draw:
        text.loadString("Draw!", 50, color::WHITE);
        break;
    case GameState::Stalemate:
        text.loadString("Stalemate!", 50, color::WHITE);
        break;

    default:
        break;
    }

    SDL_Rect rect = {wsize.width / 2 - text.getWidth() / 2, wsize.height / 2 - text.getHeight() / 2, text.getWidth(), text.getHeight()};
    text.draw(NULL, &rect);

    Texture buttonText(game->getRenderer());
    buttonText.loadString("Main menu", 50, color::BLACK);
    SDL_Rect button_rect = {wsize.width / 2 - buttonText.getWidth() / 2, wsize.height / 2 + text.getHeight() / 2, buttonText.getWidth(), buttonText.getHeight()};
    menubutton = button_rect;
    SDL_SetRenderDrawColor(game->getRenderer(), color::GREEN.r, color::GREEN.g, color::GREEN.b, color::GREEN.a);
    SDL_RenderFillRect(game->getRenderer(), &button_rect);
    buttonText.draw(NULL, &button_rect);
}

void GameOver::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if (hasClickedInsideButton(x, y, menubutton))
        {
            game->pushScene(new GameMenu(game));
            //
        }
    }
}

void GameOver::update()
{
}