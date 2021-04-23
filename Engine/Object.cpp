#include "Object.h"

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

void Object::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, m_texture, nullptr, &m_pos);
}
