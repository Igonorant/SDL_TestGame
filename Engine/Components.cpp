#include "Components.h"
#include <assert.h>
#include <cmath>

Object::Object(const InitList &init)
    : m_texture(init.texture), m_pos(init.pos), m_vx(init.vx), m_vy(init.vy),
      m_state(init.state) {
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
  m_state = init.state;
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
      setState(ObjState::Moving);
      break;
    case KbdEvents::Down_KeyDown:
      setVelocityY(0.2f);
      setState(ObjState::Moving);
      break;
    case KbdEvents::Left_KeyDown:
      setVelocityX(-0.2f);
      setState(ObjState::Moving);
      break;
    case KbdEvents::Right_KeyDown:
      setVelocityX(0.2f);
      setState(ObjState::Moving);
      break;
    case KbdEvents::Up_KeyUp:
    case KbdEvents::Down_KeyUp:
    case KbdEvents::Left_KeyUp:
    case KbdEvents::Right_KeyUp:
      setVelocity(0, 0);
      setState(ObjState::Idle);
      break;
    case KbdEvents::LCtrl_KeyDown:
      setState(ObjState::Firing);
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
  if (getState() == ObjState::Firing) {
    m_bulletTimer_ms += dt_ms;
  } else {
    m_bulletTimer_ms = 0;
  }

  // Basic object update
  Object::update(dt_ms);
}

bool Player::shouldSpawnBullet() {
  if (getState() == ObjState::Firing && m_bulletTimer_ms >= m_fireRate_ms) {
    m_bulletTimer_ms %= m_fireRate_ms;
    return true;
  }
  return false;
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
