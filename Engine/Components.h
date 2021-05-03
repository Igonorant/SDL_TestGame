#pragma once

#include "../Game/Definitions.h"
#include "Enums.h"
#include <SDL2/SDL.h>
#include <vector>

class Object {
public:
  Object();
  Object(SDL_Texture *texture, const SDL_Rect &pos);

public:
  // Movement and position
  void setPos(const float x, const float y);
  void setVelocity(const float vx, const float vy);
  void setVelocityX(const float vx) { m_vx = vx; }
  void setVelocityY(const float vy) { m_vy = vy; }
  float getPosX() const { return m_pos.x; }
  float getPosY() const { return m_pos.y; }
  int getWidth() const { return m_pos.w; }
  int getHeight() const { return m_pos.h; }
  float getVelocityX() const { return m_vx; }
  float getVelocityY() const { return m_vy; }

  // Render related
  void setTexture(SDL_Texture *texture);
  void render(SDL_Renderer *renderer);

  // Others
  virtual void update(const int dt_ms);
  void scale(const float factor);
  bool isColiding(const Object &obj);
  void setState(const ObjState state) { m_state = state; }
  ObjState getState() { return m_state; }

private:
  SDL_Texture *m_texture;
  SDL_Rect m_pos;
  float m_vx = 0;
  float m_vy = 0;
  ObjState m_state = ObjState::Idle;
};

class Player : public Object {
public:
  Player();
  Player(SDL_Texture *texture, const SDL_Rect &rect);

public:
  void update(const int dt_ms, const std::vector<KbdEvents> &events);
  bool shouldSpawnBullet();

private:
  int m_health = 100;
  int m_bulletTimer_ms = 0;
  int m_fireRate_ms = 100; // fire every X ms
};

class Projectile : public Object {
public:
  Projectile();
  Projectile(SDL_Texture *texture, const float x, const float y, const float vx,
             const float vy, const int lifespan_ms, const int damage);

public:
  void update(const int dt_ms);
  bool endedLifespan() const { return m_endedLifespan; }
  void hitted();

private:
  int m_lifespan_ms = 0;
  int m_damage = 0;
  bool m_endedLifespan = false;
};

class Timer {
public:
  Timer();
  Timer(const Uint32 interval);

public:
  void setInterval(const Uint32 interval) { m_interval_ms = interval; }
  bool triggered();
  void waitUntilNextTrigger();
  Uint32 getTimeSinceLastCall();

private:
  Uint32 m_interval_ms = 100;
  Uint32 m_lastTick_ms = 0;
  Uint32 m_lastCall_ms = 0;
};
