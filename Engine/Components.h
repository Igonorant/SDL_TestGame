#pragma once

#include "Enums.h"
#include <SDL2/SDL.h>
#include <vector>

class Object {
public:
  Object();
  Object(SDL_Texture *texture);
  Object(SDL_Texture *texture, const float x, const float y);
  Object(SDL_Texture *texture, const SDL_Rect &rect);

public:
  void setTexture(SDL_Texture *texture);
  void setPos(const float x, const float y);
  void render(SDL_Renderer *renderer);
  virtual void update(const int dt_ms);
  void updatePos(const float x, const float y);
  void setVelocity(const float vx, const float vy);
  void setVelocityX(const float vx);
  void setVelocityY(const float vy);
  float getPosX() const { return m_pos.x; }
  float getPosY() const { return m_pos.y; }
  float getVelocityX() const { return m_vx; }
  float getVelocityY() const { return m_vy; }
  void scale(const float factor);

private:
  SDL_Texture *m_texture;
  SDL_Rect m_pos;
  float m_vx = 0;
  float m_vy = 0;
};

class Player : public Object {
public:
  Player();
  Player(SDL_Texture *texture, const SDL_Rect &rect);

public:
  void update(const int dt_ms, const std::vector<KbdEvents> &events);

private:
  int health = 100;
};

class Projectile : public Object {
public:
  Projectile();
  Projectile(SDL_Texture *texture, const float x, const float y, const float vx,
             const float vy, const int lifespan_ms, const int damage);

public:
  void update(const int dt_ms);
  bool endedLifespan() const { return m_endedLifespan; }

private:
  int m_lifespan_ms = 0;
  int m_damage = 0;
  bool m_endedLifespan = false;
};
