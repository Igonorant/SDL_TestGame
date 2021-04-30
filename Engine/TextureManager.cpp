#include "TextureManager.h"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>

TextureManager::TextureManager(SDL_Renderer *renderer)
    : m_SDL_Renderer(renderer) {
  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {
    std::cout << "Couldn't initialize SDL Image: " << SDL_GetError()
              << std::endl;
  }
}

TextureManager::~TextureManager() {
  for (auto &it : m_loadedTextures) {
    SDL_DestroyTexture(it.second);
    it.second = nullptr;
  }
}

SDL_Texture *TextureManager::GetTexture(const std::string &filePath) {
  auto it = m_loadedTextures.find(filePath);
  if (it == m_loadedTextures.end()) {
    SDL_Texture *texture = IMG_LoadTexture(m_SDL_Renderer, filePath.c_str());
    m_loadedTextures.emplace(std::make_pair(filePath, texture));
    return texture;
  } else {
    return it->second;
  }
}