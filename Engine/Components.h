#pragma once

#include "Enums.h"
#include <SDL2/SDL.h>
#include <set>
#include <unordered_map>
#include <vector>

class Animation {
  struct Frame {
    SDL_Texture *m_texture;
    SDL_Rect m_frame;
    Uint32 m_time;
  };

public:
  Animation() = default;
  Animation(const std::vector<Animation::Frame> &animation);

public:
  void addFrame(const Frame &frame);
  void addFrames(const std::vector<Animation::Frame> &frames);
  void render(SDL_Renderer *renderer, const SDL_Rect &dst);
  void update(const Uint32 dt);
  void reset();

private:
  std::vector<Animation::Frame> m_frames;
  Uint32 m_currTime_ms = 0;
  unsigned int m_currFrame = 0;
  unsigned int m_nFrames = 0;
};

class Object {
public:
  struct InitList {
    SDL_Texture *texture = nullptr;
    const float scale = 1.0f;
    const SDL_Rect &pos = {0, 0, 0, 0};
    const bool queryTexture = false;
    const float vx = 0.0f;
    const float vy = 0.0f;
    const std::vector<ObjState> states = {ObjState::Idle};
  };

public:
  Object() = default;
  Object(const InitList &init);
  void initialize(const InitList &init);

public:
  // Movement and position
  void setPos(const float x, const float y);
  void setVelocity(const float vx, const float vy);
  void setVelocityX(const float vx) { m_vx = vx; }
  void setVelocityY(const float vy) { m_vy = vy; }
  SDL_Rect getPos() const { return m_pos; }
  float getPosX() const { return m_pos.x; }
  float getPosY() const { return m_pos.y; }
  int getWidth() const { return m_pos.w; }
  int getHeight() const { return m_pos.h; }
  float getVelocityX() const { return m_vx; }
  float getVelocityY() const { return m_vy; }

  // Render related
  void setTexture(SDL_Texture *texture);
  virtual void render(SDL_Renderer *renderer);

  // Others
  virtual void update(const Uint32 dt_ms);
  void scale(const float factor);
  bool isColiding(const Object &obj);

  // State
  void setStates(const std::vector<ObjState> &states) { m_states = states; }
  const std::vector<ObjState> &getStates() const { return m_states; }
  void addState(const ObjState state);
  void removeState(const ObjState state);
  void replaceState(const ObjState state, const ObjState toReplace,
                    const bool addIfNotFound = true);
  bool hasState(const ObjState state) const;

private:
  SDL_Texture *m_texture;
  SDL_Rect m_pos;
  float m_vx = 0;
  float m_vy = 0;
  std::vector<ObjState> m_states = {ObjState::Idle};
};

class Player : public Object {
public:
  enum class AnimationState { Idle, Firing, Moving, FiringAndMoving };

public:
  Player() = default;
  Player(const Object::InitList &init, const int health, const int fireRate);
  void initialize(const Object::InitList &init, const int health,
                  const int fireRate);

public:
  void update(const Uint32 dt_ms, const std::vector<KbdEvents> &events);
  bool shouldSpawnBullet();
  void addAnimation(const Player::AnimationState state,
                    const Animation &animation);
  void render(SDL_Renderer *renderer) override;

private:
  int m_health = 100;
  Uint32 m_bulletTimer_ms = 0;
  Uint32 m_fireRate_ms = 100; // fire every X ms
  AnimationState m_animationState = AnimationState::Idle;
  std::unordered_map<AnimationState, Animation> m_animations;
};

class Projectile : public Object {
public:
  Projectile() = default;
  Projectile(const Object::InitList &init, const int lifespan_ms,
             const int damage);
  void initialize(const Object::InitList &init, const int lifespan_ms,
                  const int damage);

public:
  void update(const Uint32 dt_ms);
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
