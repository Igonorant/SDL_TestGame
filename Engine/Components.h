#pragma once

#include "Components_forward.h"
#include <SDL2/SDL.h>
#include <set>
#include <unordered_map>
#include <vector>

class Frame {
public:
  Frame() = default;
  Frame(SDL_Texture *texture, const SDL_Rect &source,
        const bool queryTexture = false);

public:
  void render(SDL_Renderer *renderer, const SDL_Rect &destination);

  // Setters
  void setTexture(SDL_Texture *texture, const bool queryTexture = false);
  void setSource(const SDL_Rect &source, const bool queryTexture = false);

  // Getters
  SDL_Texture *getTexture() const { return m_texture; }
  const SDL_Rect &getSource() const { return m_src; }

private:
  SDL_Texture *m_texture = nullptr;
  SDL_Rect m_src;
};

class AnimationFrame {
public:
  AnimationFrame() = default;
  AnimationFrame(const Frame &frame, const Uint32 ticks);
  AnimationFrame(SDL_Texture *texture, const SDL_Rect &source,
                 const Uint32 &ticks, const bool queryTexture = false);

public:
  void render(SDL_Renderer *renderer, const SDL_Rect &destination);

  // Setters
  void setFrame(const Frame &frame) { m_frame = frame; }
  void setTicks(const Uint32 &ticks) { m_ticks = ticks; }

  // Getters
  const Frame &getFrame() const { return m_frame; }
  const Uint32 &getTicks() const { return m_ticks; }

private:
  Frame m_frame;
  Uint32 m_ticks;
};

class Animation {
public:
  Animation() = default;
  Animation(const std::vector<AnimationFrame> &animation);

public:
  void addFrame(const AnimationFrame &frame);
  void addFrames(const std::vector<AnimationFrame> &frames);
  void render(SDL_Renderer *renderer, const SDL_Rect &destination);
  void update(const Uint32 &dt);
  void reset();

private:
  std::vector<AnimationFrame> m_frames;
  Uint32 m_currTicks = 0;
  unsigned int m_currFrame = 0;
  unsigned int m_nFrames = 0;
};

class Object {
public:
  Object() = default;
  Object(const Animation &animation, const Rectf &destination,
         const float scale = 1.0f, const ObjState state = ObjState::Idle);

public:
  virtual void update(const Uint32 &dt);
  virtual void render(SDL_Renderer *renderer);
  void scale(const float factor);
  bool isColiding(const Object &obj);
  void updatePosX(const float dx) { m_dst.x += dx; }
  void updatePosY(const float dy) { m_dst.y += dy; }

  // Setters
  void setState(const ObjState state) { m_state = state; }
  void setPos(const float x, const float y) {
    m_dst.x = x;
    m_dst.y = y;
  }
  void setPosX(const float x) { m_dst.x = x; }
  void setPosY(const float y) { m_dst.y = y; }
  void setDestination(const Rectf &dst) { m_dst = dst; }
  void setAnimation(const Animation &animation) { m_animation = animation; }

  // Getters
  ObjState getState() const { return m_state; }
  float getPosX() const { return m_dst.x; }
  float getPosY() const { return m_dst.y; }
  float getWidth() const { return m_dst.w; }
  float getHeight() const { return m_dst.h; }
  float getOppositeX() const { return m_dst.x + m_dst.w; }
  float getOppositeY() const { return m_dst.y + m_dst.h; }
  const Rectf &getDestination() const { return m_dst; }
  const SDL_Rect getAbsoluteDestination() const;

private:
  Animation m_animation;
  Rectf m_dst;
  ObjState m_state = ObjState::Idle;
};

class DynamicObject : public Object {
public:
  DynamicObject() = default;
  DynamicObject(const std::unordered_map<ObjState, Animation> &animations,
                const Rectf &destination, const float vx = 0.0f,
                const float vy = 0.0f, const bool gravitySensitive = true,
                const float scale = 1.0f,
                const ObjState state = ObjState::Idle);

public:
  // Others
  void update(const Uint32 &dt) override;
  void render(SDL_Renderer *renderer) override;
  void addAnimation(const ObjState state, const Animation &animation);

  // Setters
  void setVelocity(const float vx, const float vy) {
    m_vx = vx;
    m_vy = vy;
  }
  void setVelocityX(const float vx) { m_vx = vx; }
  void setVelocityY(const float vy) { m_vy = vy; }
  void setGravitySensitive(const bool gravStv) { m_gravitySensitive = gravStv; }

  // Getters
  float getVelocityX() const { return m_vx; }
  float getVelocityY() const { return m_vy; }

private:
  float m_vx = 0.0f;
  float m_vy = 0.0f;
  bool m_gravitySensitive = true;
  std::unordered_map<ObjState, Animation> m_animations;
  ObjState m_previousState = ObjState::Idle;
};

class Player : public DynamicObject {
public:
  Player() = default;
  Player(const std::unordered_map<ObjState, Animation> &animations,
         const Rectf &destination, const int health, const Uint32 &fireRate,
         const float vx = 0.0f, const float vy = 0.0f,
         const bool gravitySensitive = true, const float scale = 1.0f,
         const ObjState state = ObjState::Idle);

public:
  void update(const Uint32 &dt, const std::vector<KbdEvents> &events);
  bool shouldSpawnBullet();

  // Setters
  void setHealth(const int health) { m_health = health; }
  void setFireRate(const int fireRate) { m_fireRate = 1000 / fireRate; }

private:
  int m_health = 100;
  Uint32 m_bulletTimer = 0;
  Uint32 m_fireRate = 100; // fire every X ticks
};

class Projectile : public DynamicObject {
public:
  Projectile() = default;
  Projectile(const std::unordered_map<ObjState, Animation> &animations,
             const Rectf &destination, const int damage, const Uint32 &lifeSpan,
             const float vx = 0.0f, const float vy = 0.0f,
             const bool gravitySensitive = true, const float scale = 1.0f,
             const ObjState state = ObjState::Moving);

public:
  void update(const Uint32 &dt);
  bool endedLifespan() const { return m_endedLifespan; }
  void hitted();

  // Setters
  void setLifeSpan(const Uint32 &lifeSpan) { m_lifeSpan = lifeSpan; }
  void setDamage(const int damage) { m_damage = damage; }

private:
  Uint32 m_lifeSpan = 0;
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
