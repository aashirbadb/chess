#pragma once
#include "gameScene.h"
#include "board.h"
#include "piece.h"
#include "stockfish.h"
#include "player.h"
#include <unordered_map>
#include "gameMenu.h"

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

public:
    ChessGame(Game *g);
    ChessGame(Game *g, bool isWhiteHuman, bool isBlackHuman, std::string fen = STARTING_FEN, int difficulty = 10);

    ~ChessGame() override;
    void render() override;
    void handleEvent(SDL_Event &e) override;
    void update() override;
    void handleResize() override;

private:
    void renderBoard();
    void renderPromotionMenu();
    void renderTile(Coordinate coord, SDL_Color color);
    void renderPiece(Piece *piece);
    void handlePromotion(int x, int y);
    void handleBoardClick(Coordinate);

    void playSound(MoveType _type);
    void loadTextures();

    Player *getCurrentPlayer();
};
