#include "Object.h"


Object::Object()
    : Object(nullptr)
{
}

Object::Object(SDL_Texture* texture)
    : Object(texture, 0, 0)
{
}

Object::Object(SDL_Texture* texture, const int x, const int y)
    : Object(texture, {x, y})
{
}

Object::Object(SDL_Texture* texture, const SDL_Rect& rect)
    :m_texture(texture)
{
    m_pos = rect;
	SDL_QueryTexture(m_texture, nullptr, nullptr, &m_pos.w, &m_pos.h);
}

void Object::setTexture(SDL_Texture* texture)
{
    m_texture = texture;
    SDL_QueryTexture(m_texture, nullptr, nullptr, &m_pos.w, &m_pos.h);
}

void Object::setPos(const int x, const int y)
{
    m_pos.x = x;
    m_pos.y = y;
}

void Object::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, m_texture, nullptr, &m_pos);
}

void Object::updatePos(const int x, const int y)
{
    m_pos.x += x;
    m_pos.y += y;
}

