#include "Components.h"
#include <assert.h>
#include <cmath>


Object::Object() : Object(nullptr) {}

Object::Object(SDL_Texture *texture) : Object(texture, 0, 0) {}

Object::Object(SDL_Texture *texture, const int x, const int y)
    : Object(texture, {x, y}) {}

Object::Object(SDL_Texture *texture, const SDL_Rect &rect)
    : m_texture(texture) {
  m_pos = rect;
  SDL_QueryTexture(m_texture, nullptr, nullptr, &m_pos.w, &m_pos.h);
}

void Object::setTexture(SDL_Texture *texture) {
  m_texture = texture;
  SDL_QueryTexture(m_texture, nullptr, nullptr, &m_pos.w, &m_pos.h);
}

void Object::setPos(const int x, const int y) {
  m_pos.x = x;
  m_pos.y = y;
}

void Object::render(SDL_Renderer *renderer) {
  SDL_RenderCopy(renderer, m_texture, nullptr, &m_pos);
}

void Object::update() {
  m_pos.x += m_vx;
  m_pos.y += m_vy;
}

void Object::updatePos(const int x, const int y) {
  m_pos.x += x;
  m_pos.y += y;
}

void Object::setVelocity(const int vx, const int vy) {
  m_vx = vx;
  m_vy = vy;
}

void Object::setVelocityX(const int vx) { m_vx = vx; }

void Object::setVelocityY(const int vy) { m_vy = vy; }

Projectile::Projectile() : Object(nullptr) {}

Projectile::Projectile(SDL_Texture *texture, const int x, const int y,
                       const int vx, const int vy, const int lifespan_ms,
                       const int damage)
    : Object(texture, x, y), m_lifespan_ms(lifespan_ms), m_damage(damage) {
  setVelocity(vx, vy);
}

void Projectile::update(const int dt_ms) {
  Object::update();
  m_lifespan_ms -= dt_ms;
  m_endedLifespan = m_lifespan_ms <= 0;
}

void Object::scale(const float factor) {
  assert(factor > 0);
  m_pos.h = std::ceil(m_pos.h * factor);
  m_pos.w = std::ceil(m_pos.w * factor);
}
