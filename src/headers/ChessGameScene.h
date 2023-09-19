#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include <SDL2/SDL.h>

class Board;
class Piece;

enum class GameState;

#include "texture.h"
#include "player.h"
#include "coordinate.h"
#include "gameScene.h"

#include "board.h"

class ChessGame : public GameScene
{
    Board *board;
    Piece *selected_piece;
    bool playing;
    static char rowlabel[9], columnlabel[9], promotionPieces[4];
    Player *players[2];
    GameState state;

    std::unordered_map<char, Texture> pieceTextures;
    std::unordered_map<GameState, Texture> stateTextures;
    std::vector<Texture *> rankTexture;
    std::vector<Texture *> fileTexture;
    Texture playerNameTextures[2];

    Texture quitTexture;
    Texture resignTexture;
    Texture replayTexture;

    SDL_Rect quitButtonRect;
    SDL_Rect resignButtonRect;
    SDL_Rect replayButtonRect;

    bool isSelectedPieceDrag;
    Uint64 clickStart;

public:
    ChessGame(Game *g, bool isWhiteHuman = true, bool isBlackHuman = true, std::string fen = STARTING_FEN, int difficulty = 10);

    ~ChessGame() override;
    void render() override;
    void handleEvent(SDL_Event &e) override;
    void handleLeftMouse(SDL_Event &e) override;
    void update() override;
    void handleResize() override;

private:
    void renderBoard();
    void renderPromotionMenu();
    void renderTile(Coordinate coord, SDL_Color color);
    void renderPiece(Piece *piece);
    void handlePromotion(int x, int y);
    void handleBoardClick(Coordinate);

    void playSound(Move _move);
    void loadTextures();

    Player *getCurrentPlayer();
};
