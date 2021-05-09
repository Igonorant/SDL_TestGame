#include "Components.h"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <numeric>

Object::Object(const InitList &init)
    : m_texture(init.texture), m_pos(init.pos), m_vx(init.vx), m_vy(init.vy),
      m_states(init.states) {
  if (init.queryTexture) {
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_pos.w, &m_pos.h);
  }
  m_pos.h *= init.scale;
  m_pos.w *= init.scale;
}

void Object::initialize(const InitList &init) {
  m_texture = init.texture;
  m_pos = init.pos;
  if (init.queryTexture) {
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_pos.w, &m_pos.h);
  }
  m_pos.h *= init.scale;
  m_pos.w *= init.scale;
  m_vx = init.vx;
  m_vy = init.vy;
  m_states = init.states;
}

void Object::setTexture(SDL_Texture *texture) {
  m_texture = texture;
  SDL_QueryTexture(m_texture, nullptr, nullptr, &m_pos.w, &m_pos.h);
}

void Object::setPos(const float x, const float y) {
  m_pos.x = x;
  m_pos.y = y;
}

void Object::render(SDL_Renderer *renderer) {
  SDL_RenderCopy(renderer, m_texture, nullptr, &m_pos);
}

void Object::update(const Uint32 dt_ms) {
  m_pos.x += m_vx * dt_ms;
  m_pos.y += m_vy * dt_ms;
}

void Object::setVelocity(const float vx, const float vy) {
  setVelocityX(vx);
  setVelocityY(vy);
}

void Object::addState(const ObjState state) {
  if (std::find(m_states.cbegin(), m_states.cend(), state) == m_states.cend()) {
    m_states.push_back(state);
  }
}

void Object::removeState(const ObjState state) {
  const auto it = std::find(m_states.cbegin(), m_states.cend(), state);
  if (it != m_states.cend()) {
    m_states.erase(it);
  }
}

void Object::replaceState(const ObjState state, const ObjState toReplace,
                          const bool addIfNotFound) {
  const auto it = std::find(m_states.cbegin(), m_states.cend(), toReplace);
  if (it != m_states.cend()) {
    m_states.erase(it);
    m_states.push_back(state);
  } else if (addIfNotFound) {
    m_states.push_back(state);
  }
}

bool Object::hasState(const ObjState state) const {
  return std::find(m_states.cbegin(), m_states.cend(), state) !=
         m_states.cend();
}

Projectile::Projectile(const Object::InitList &init, const int lifespan_ms,
                       const int damage)
    : Object(init), m_lifespan_ms(lifespan_ms), m_damage(damage) {}

void Projectile::initialize(const Object::InitList &init, const int lifespan_ms,
                            const int damage) {
  Object::initialize(init);
  m_lifespan_ms = lifespan_ms;
  m_damage = damage;
}

void Projectile::update(const Uint32 dt_ms) {
  Object::update(dt_ms);
  m_lifespan_ms -= dt_ms;
  m_endedLifespan = m_lifespan_ms <= 0;
}

void Projectile::hitted() {
  m_lifespan_ms = 0;
  m_endedLifespan = true;
}

void Object::scale(const float factor) {
  assert(factor > 0);
  m_pos.h = std::ceil(m_pos.h * factor);
  m_pos.w = std::ceil(m_pos.w * factor);
}

bool Object::isColiding(const Object &obj) {
  struct vertex {
    float x;
    float y;
  };

  const vertex currTopLeft = {m_pos.x, m_pos.y};
  const vertex currBottomRight = {m_pos.x + m_pos.w, m_pos.y + m_pos.h};
  const vertex objTopLeft = {obj.getPosX(), obj.getPosY()};
  const vertex objBottomRight = {obj.getPosX() + obj.getWidth(),
                                 obj.getPosY() + obj.getHeight()};

  return currTopLeft.x <= objBottomRight.x &&
         currTopLeft.y <= objBottomRight.y &&
         currBottomRight.x >= objBottomRight.x &&
         currBottomRight.y >= objBottomRight.y;
}

Player::Player(const Object::InitList &init, const int health,
               const int fireRate)
    : Object(init), m_health(health), m_fireRate_ms(1000 / fireRate) {}

void Player::initialize(const Object::InitList &init, const int health,
                        const int fireRate) {
  Object::initialize(init);
  m_health = health;
  m_fireRate_ms = 1000 / fireRate;
}

void Player::update(const Uint32 dt_ms, const std::vector<KbdEvents> &events) {
  // Handle events
  for (const auto event : events) {
    switch (event) {
    case KbdEvents::Up_KeyDown:
      setVelocityY(-0.2f);
      replaceState(ObjState::Moving, ObjState::Idle);
      break;
    case KbdEvents::Down_KeyDown:
      setVelocityY(0.2f);
      replaceState(ObjState::Moving, ObjState::Idle);
      break;
    case KbdEvents::Left_KeyDown:
      setVelocityX(-0.2f);
      replaceState(ObjState::Moving, ObjState::Idle);
      break;
    case KbdEvents::Right_KeyDown:
      setVelocityX(0.2f);
      replaceState(ObjState::Moving, ObjState::Idle);
      break;
    case KbdEvents::Up_KeyUp:
    case KbdEvents::Down_KeyUp:
      setVelocityY(0.0f);
      if (getVelocityX() == 0.0f) {
        replaceState(ObjState::Idle, ObjState::Moving);
      }
      break;
    case KbdEvents::Left_KeyUp:
    case KbdEvents::Right_KeyUp:
      setVelocityX(0.0f);
      if (getVelocityY() == 0.0f) {
        replaceState(ObjState::Idle, ObjState::Moving);
      }
      break;
    case KbdEvents::LCtrl_KeyDown:
      addState(ObjState::Firing);
      break;
    case KbdEvents::LCtrl_KeyUp:
      removeState(ObjState::Firing);
      break;
    default:
      break;
    }
  }

  // Update bullet timer
  if (hasState(ObjState::Firing)) {
    m_bulletTimer_ms += dt_ms;
  } else {
    m_bulletTimer_ms = 0;
  }

  // Update animations
  const auto previousAnimState = m_animationState;
  if (hasState(ObjState::Idle)) {
    m_animationState = hasState(ObjState::Firing) ? AnimationState::Firing
                                                  : AnimationState::Idle;
  } else if (hasState(ObjState::Moving)) {
    m_animationState = hasState(ObjState::Firing)
                           ? AnimationState::FiringAndMoving
                           : AnimationState::Moving;
  }
  if (previousAnimState != m_animationState) {
    m_animations[m_animationState].reset();
  }
  m_animations[m_animationState].update(dt_ms);

  // Basic object update
  Object::update(dt_ms);
}

bool Player::shouldSpawnBullet() {
  if (hasState(ObjState::Firing) && m_bulletTimer_ms >= m_fireRate_ms) {
    m_bulletTimer_ms %= m_fireRate_ms;
    return true;
  }
  return false;
}

void Player::addAnimation(const Player::AnimationState state,
                          const Animation &animation) {
  m_animations.insert(std::make_pair(state, animation));
}

void Player::render(SDL_Renderer *renderer) {
  m_animations[m_animationState].render(
      renderer, {int(getPosX()), int(getPosY()), getWidth(), getHeight()});
}

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
