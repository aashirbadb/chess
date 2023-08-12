#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>

typedef std::unordered_map<int, TTF_Font *> FontMap;

class Texture
{
    int h, w;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    static FontMap FONTS;

public:
    Texture(SDL_Renderer *r);
    ~Texture();

    int getHeight() { return h; }
    int getWidth() { return w; }

    TTF_Font *getFont(int fontsize);

    void loadChar(char ch, int fontsize, SDL_Color color);
    void loadString(const char *str, int fontsize, SDL_Color color);

    void draw(SDL_Rect *src, SDL_Rect *dest);

    static void clearTextures();
};