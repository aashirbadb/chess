#include "headers/texture.h"
#include <iostream>

FontMap Texture::FONTS;

Texture::Texture()
{
    renderer = nullptr;
    texture = nullptr;
    h = 0;
    w = 0;
    rect = {0, 0, 0, 0};
}

Texture::Texture(SDL_Renderer *r)
{
    renderer = r;
    texture = nullptr;
    h = 0;
    w = 0;
    rect = {0, 0, 0, 0};
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
        // std::cerr << "Font of size " << fontsize << " not found. Creating new one." << std::endl;
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
    rect = text->clip_rect;

    texture = SDL_CreateTextureFromSurface(renderer, text);

    SDL_FreeSurface(text);
}

void Texture::loadString(std::string str, int fontsize, SDL_Color color)
{
    TTF_Font *font = getFont(fontsize);

    SDL_Surface *text = TTF_RenderText_Blended(font, str.c_str(), color);

    h = text->h;
    w = text->w;
    rect = text->clip_rect;

    texture = SDL_CreateTextureFromSurface(renderer, text);

    SDL_FreeSurface(text);
}

void Texture::draw(SDL_Rect *src, SDL_Rect *dest)
{
    SDL_RenderCopy(renderer, texture, src, dest);
}

void Texture::draw(int x, int y)
{
    SDL_Rect dest = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Texture::drawCentered(SDL_Rect rect)
{
    SDL_Rect centered = {
        rect.x + (rect.w - w) / 2,
        rect.y + (rect.h - h) / 2,
        w,
        h,
    };

    SDL_RenderCopy(renderer, texture, NULL, &centered);
}

SDL_Rect Texture::getRect()
{
    return rect;
}

void Texture::clearTextures()
{
    for (auto &it : Texture::FONTS)
    {
        TTF_CloseFont(it.second); //This causes segfault🤷🤷
    }
}

void Texture::setRenderer(SDL_Renderer *r)
{
    renderer = r;
}

void Texture::loadImage(const char *path)
{
    texture = IMG_LoadTexture(renderer, path);
    if (texture == nullptr)
    {
        std::cerr << "Error loading " << path << ": " << IMG_GetError() << std::endl;
    }
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

SDL_Texture *Texture::getTexture()
{
    return texture;
}

void Texture::drawCircle(SDL_Renderer *_renderer, int x, int y, int r, SDL_Color color)
{
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < r * 2; w++)
    {
        for (int h = 0; h < r * 2; h++)
        {
            int dx = r - w; // horizontal offset
            int dy = r - h; // vertical offset
            if ((dx * dx + dy * dy) <= (r * r))
            {
                SDL_RenderDrawPoint(_renderer, x + dx, y + dy);
            }
        }
    }
}
