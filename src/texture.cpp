#include "headers/texture.h"
#include <iostream>

FontMap Texture::FONTS;

Texture::Texture(SDL_Renderer *r)
{
    renderer = r;
    texture = nullptr;
    h = 0;
    w = 0;
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}

TTF_Font *Texture::getFont(int fontsize)
{
    TTF_Font *font = Texture::FONTS[fontsize];

    if (font == nullptr)
    {
        std::cout << "Font of size " << fontsize << " not found. Opening new one." << std::endl;
        font = TTF_OpenFont("assets/Roboto.ttf", fontsize);
        FONTS[fontsize] = font;
    }
    return font;
}

void Texture::loadChar(char ch, int fontsize, SDL_Color color)
{
    TTF_Font *font = getFont(fontsize);

    SDL_Surface *text = TTF_RenderGlyph32_Blended(font, ch, color);

    h = text->h;
    w = text->w;

    texture = SDL_CreateTextureFromSurface(renderer, text);

    SDL_FreeSurface(text);
}

void Texture::loadString(const char *str, int fontsize, SDL_Color color)
{
    TTF_Font *font = getFont(fontsize);

    SDL_Surface *text = TTF_RenderText_Blended(font, str, color);

    h = text->h;
    w = text->w;

    texture = SDL_CreateTextureFromSurface(renderer, text);

    SDL_FreeSurface(text);
}

void Texture::draw(SDL_Rect *src, SDL_Rect *dest)
{
    SDL_RenderCopy(renderer, texture, src, dest);
}

void Texture::clearTextures()
{
    for (auto &it : Texture::FONTS)
    {
        // std::cout <<it.first <<std::endl;
        // TTF_CloseFont(it.second); //This causes segfault🤷🤷
    }
}