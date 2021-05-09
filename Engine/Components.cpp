#include "Components.h"
#include "../Game/Definitions.h"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <numeric>

// OBJECT STAR
Object::Object(const Animation &animation, const Rectf &destination,
               const float scale, const ObjState state)
    : m_animation(animation), m_dst(destination), m_state(state) {
  m_dst.w *= scale;
  m_dst.h *= scale;
}

const SDL_Rect Object::getAbsoluteDestination() const {
  int x = int(m_dst.x * Global::SDL::ScreenWidth);
  int y = int(m_dst.y * Global::SDL::ScreenHeight);
  int w = int(m_dst.w * Global::SDL::ScreenWidth);
  int h = int(m_dst.h * Global::SDL::ScreenHeight);
  return {x, y, w, h};
}

void Object::render(SDL_Renderer *renderer) {
  m_animation.render(renderer, getAbsoluteDestination());
}

void Object::update(const Uint32 &dt) { m_animation.update(dt); }

void Object::scale(const float factor) {
  m_dst.w *= factor;
  m_dst.h *= factor;
}

bool Object::isColiding(const Object &obj) {
  return getPosX() <= obj.getPosX() && getPosY() <= obj.getPosY() &&
         getOppositeX() >= obj.getOppositeX() &&
         getOppositeY() >= obj.getOppositeY();
}
// OBJECT END

// DYNAMIC OBJECT START
DynamicObject::DynamicObject(
    const std::unordered_map<ObjState, Animation> &animations,
    const Rectf &destination, const float vx, const float vy,
    const bool gravitySensitive, const float scale, const ObjState state)
    : Object(Animation(), destination, scale, state), m_animations(animations),
      m_vx(vx), m_vy(vy), m_gravitySensitive(gravitySensitive),
      m_previousState(state) {}

void DynamicObject::update(const Uint32 &dt) {
  updatePosX(m_vx * dt);
  updatePosY(m_vy * dt);
  if (getOppositeY() >= Global::Game::Floor) {
    setPosY(Global::Game::Floor - getHeight());
    m_vy = 0.0f;
  } else if (m_gravitySensitive) {
    m_vy += Global::Game::Gravity;
  }

  // Update animations
  if (m_previousState != getState()) {
    m_animations[getState()].reset();
  }
  m_animations[getState()].update(dt);
  m_previousState = getState();
}

void DynamicObject::render(SDL_Renderer *renderer) {
  m_animations[getState()].render(renderer, getAbsoluteDestination());
}

void DynamicObject::addAnimation(const ObjState state,
                                 const Animation &animation) {
  m_animations.insert(std::make_pair(state, animation));
}
// DYNAMIC OBJECT END

// PLAYER BEGIN
Player::Player(const std::unordered_map<ObjState, Animation> &animations,
               const Rectf &destination, const int health,
               const Uint32 &fireRate, const float vx, const float vy,
               const bool gravitySensitive, const float scale,
               const ObjState state)
    : DynamicObject(animations, destination, vx, vy, gravitySensitive, scale,
                    state),
      m_health(health), m_fireRate(1000 / fireRate) {}

void Player::update(const Uint32 &dt, const std::vector<KbdEvents> &events) {
  // Handle events
  const auto previousState = getState();
  for (const auto event : events) {
    switch (event) {
    case KbdEvents::Up_KeyDown:
      setVelocityY(-0.001f);
      if (getState() == ObjState::Idle) {
        setState(ObjState::Moving);
      } else if (getState() == ObjState::Firing) {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::Down_KeyDown:
      setVelocityY(0.001f);
      if (getState() == ObjState::Idle) {
        setState(ObjState::Moving);
      } else if (getState() == ObjState::Firing) {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::Left_KeyDown:
      setVelocityX(-0.001f);
      if (getState() == ObjState::Idle) {
        setState(ObjState::Moving);
      } else if (getState() == ObjState::Firing) {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::Right_KeyDown:
      setVelocityX(0.001f);
      if (getState() == ObjState::Idle) {
        setState(ObjState::Moving);
      } else if (getState() == ObjState::Firing) {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::Up_KeyUp:
    case KbdEvents::Down_KeyUp:
      if (getVelocityX() == 0.0f) {
        if (getState() == ObjState::Moving) {
          setState(ObjState::Idle);
        } else if (getState() == ObjState::FiringAndMoving) {
          setState(ObjState::Firing);
        }
      }
      break;
    case KbdEvents::Left_KeyUp:
    case KbdEvents::Right_KeyUp:
      setVelocityX(0.0f);
      if (getVelocityY() == 0.0f) {
        if (getState() == ObjState::Moving) {
          setState(ObjState::Idle);
        } else if (getState() == ObjState::FiringAndMoving) {
          setState(ObjState::Firing);
        }
      }
      break;
    case KbdEvents::LCtrl_KeyDown:
      if (getVelocityX() == 0.0f && getVelocityY() == 0.0f) {
        setState(ObjState::Firing);
      } else {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::LCtrl_KeyUp:
      if (getVelocityX() == 0.0f && getVelocityY() == 0.0f) {
        setState(ObjState::Idle);
      } else {
        setState(ObjState::Moving);
      }
      break;
    default:
      break;
    }
  }

  // Update bullet timer
  if (getState() == ObjState::Firing ||
      getState() == ObjState::FiringAndMoving) {
    m_bulletTimer += dt;
  } else {
    m_bulletTimer = 0;
  }

  // Parent class update
  DynamicObject::update(dt);
}

bool Player::shouldSpawnBullet() {
  if ((getState() == ObjState::Firing ||
       getState() == ObjState::FiringAndMoving) &&
      m_bulletTimer >= m_fireRate) {
    m_bulletTimer %= m_fireRate;
    return true;
  }
  return false;
}
// PLAYER END

// PROJECTILE START
Projectile::Projectile(
    const std::unordered_map<ObjState, Animation> &animations,
    const Rectf &destination, const int damage, const Uint32 &lifeSpan,
    const float vx, const float vy, const bool gravitySensitive,
    const float scale, const ObjState state)
    : DynamicObject(animations, destination, vx, vy, gravitySensitive, scale,
                    state),
      m_damage(damage), m_lifeSpan(lifeSpan) {}

void Projectile::update(const Uint32 &dt) {
  DynamicObject::update(dt);
  if (m_lifeSpan <= dt) {
    m_lifeSpan = 0;
    m_endedLifespan = m_lifeSpan <= 0;
  } else {
    m_lifeSpan -= dt;
  }
}

void Projectile::hitted() {
  m_lifeSpan = 0;
  m_endedLifespan = true;
}
// PROJECTILE END

// TIMER START
Timer::Timer() { m_lastTick_ms = SDL_GetTicks(); }

Timer::Timer(const Uint32 interval) : m_interval_ms(interval) {
  m_lastTick_ms = SDL_GetTicks();
}

bool Timer::triggered() {
  Uint32 currentTicks = SDL_GetTicks();
  if (currentTicks - m_lastTick_ms > m_interval_ms) {
    m_lastTick_ms = currentTicks;
    return true;
  }
  return false;
}

void Timer::waitUntilNextTrigger() {
  Uint32 currentTicks = SDL_GetTicks();
  Uint32 remainingTicks =
      std::min(m_interval_ms - (currentTicks - m_lastTick_ms), m_interval_ms);
  SDL_Delay(remainingTicks);
  m_lastTick_ms = SDL_GetTicks();
}

Uint32 Timer::getTimeSinceLastCall() {
  Uint32 currTick = SDL_GetTicks();
  Uint32 dt = currTick - m_lastCall_ms;
  m_lastCall_ms = currTick;
  return dt;
}
// TIMER END
