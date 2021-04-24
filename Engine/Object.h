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
    void update();
    void updatePos(const int x, const int y);
    void setVelocity(const int vx, const int vy);
    void setVelocityX(const int vx);
    void setVelocityY(const int vy);
    int getPosX() const { return m_pos.x; }
    int getPosY() const { return m_pos.y; }
    int getVelocityX() const { return m_vx; }
    int getVelocityY() const { return m_vy; }

  private:
    SDL_Texture* m_texture;
    SDL_Rect m_pos;
    int m_vx = 0;
    int m_vy = 0;
};

class Projectile : public Object {
  public:
    Projectile();
    Projectile(SDL_Texture* texture, const int x, const int y, const int vx,
               const int vy, const int lifespan_ms, const int damage);
  public:
    void update(const int dt_ms);
    bool endedLifespan() const { return m_endedLifespan; }
  private:
    int m_lifespan_ms = 0;
    int m_damage = 0;
    bool m_endedLifespan = false;
};
