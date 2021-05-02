#include "Components.h"
#include <assert.h>
#include <cmath>

Object::Object() : Object(nullptr, {0, 0}) {}

Object::Object(SDL_Texture *texture, const SDL_Rect &pos) : m_texture(texture) {
  m_pos = pos;
  SDL_QueryTexture(m_texture, nullptr, nullptr, &m_pos.w, &m_pos.h);
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

void Object::update(const int dt_ms) {
  m_pos.x += m_vx * dt_ms;
  m_pos.y += m_vy * dt_ms;
}

void Object::setVelocity(const float vx, const float vy) {
  m_vx = vx;
  m_vy = vy;
}

void Object::setVelocityX(const float vx) { m_vx = vx; }

void Object::setVelocityY(const float vy) { m_vy = vy; }

Projectile::Projectile() : Object() {}

Projectile::Projectile(SDL_Texture *texture, const float x, const float y,
                       const float vx, const float vy, const int lifespan_ms,
                       const int damage)
    : Object(texture, {int(x), int(y)}), m_lifespan_ms(lifespan_ms),
      m_damage(damage) {
  setVelocity(vx, vy);
}

void Projectile::update(const int dt_ms) {
  Object::update(dt_ms);
  m_lifespan_ms -= dt_ms;
  m_endedLifespan = m_lifespan_ms <= 0;
}

void Object::scale(const float factor) {
  assert(factor > 0);
  m_pos.h = std::ceil(m_pos.h * factor);
  m_pos.w = std::ceil(m_pos.w * factor);
}

Player::Player() : Object() {}

Player::Player(SDL_Texture *texture, const SDL_Rect &rect)
    : Object(texture, rect) {}

void Player::update(const int dt_ms, const std::vector<KbdEvents> &events) {
  for (const auto event : events) {
    switch (event) {
    case KbdEvents::Up_KeyDown:
      setVelocityY(-0.1f);
      break;
    case KbdEvents::Down_KeyDown:
      setVelocityY(0.1f);
      break;
    case KbdEvents::Left_KeyDown:
      setVelocityX(-0.1f);
      break;
    case KbdEvents::Right_KeyDown:
      setVelocityX(0.1f);
      break;
    case KbdEvents::Up_KeyUp:
    case KbdEvents::Down_KeyUp:
    case KbdEvents::Left_KeyUp:
    case KbdEvents::Right_KeyUp:
      setVelocity(0, 0);
      break;
    default:
      break;
    }
  }
  Object::update(dt_ms);
}
