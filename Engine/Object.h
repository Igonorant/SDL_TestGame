#pragma once

#include <SDL2/SDL.h>

class Object {
  public:
    Object(SDL_Texture* texture);
    Object(SDL_Texture* texture, const int x, const int y);
    Object(SDL_Texture* texture, const SDL_Rect& rect);

  public:
    void Render(SDL_Renderer* renderer);

  private:
    SDL_Texture* m_texture;
    SDL_Rect m_pos;
};
