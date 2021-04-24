#pragma once

#include <SDL2/SDL.h>

class Object {
  public:
    Object();
    Object(SDL_Texture* texture);
    Object(SDL_Texture* texture, const int x, const int y);
    Object(SDL_Texture* texture, const SDL_Rect& rect);

  public:
    void setTexture(SDL_Texture* texture);
    void setPos(const int x, const int y);
    void render(SDL_Renderer* renderer);
    void updatePos(const int x, const int y);

  private:
    SDL_Texture* m_texture;
    SDL_Rect m_pos;
};
