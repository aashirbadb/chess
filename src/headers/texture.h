#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>

typedef std::unordered_map<int, TTF_Font *> FontMap;

class Texture
{
    int h, w;
    SDL_Rect rect;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    static FontMap FONTS;

public:
    Texture();
    Texture(SDL_Renderer *r);
    ~Texture();

    static void drawCircle(SDL_Renderer *_renderer, int x, int y, int r, SDL_Color color);

    int getHeight() { return h; }
    int getWidth() { return w; }

    void setRenderer(SDL_Renderer *r);

    TTF_Font *getFont(int fontsize);
    SDL_Rect getRect();

    void loadChar(char ch, int fontsize, SDL_Color color);
    void loadString(std::string str, int fontsize, SDL_Color color);
    void loadImage(const char *path);

    void draw(SDL_Rect *src, SDL_Rect *dest);
    void draw(int x, int y);
    void drawCentered(SDL_Rect rect);

    SDL_Texture *getTexture();

    static void clearTextures();
};