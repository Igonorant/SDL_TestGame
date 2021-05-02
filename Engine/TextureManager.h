#pragma once

#include "../Game/Definitions.h"
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class TextureManager {
public:
  TextureManager(SDL_Renderer *renderer);
  ~TextureManager();
  TextureManager(const TextureManager &) = delete; // no copy
  TextureManager &
  operator=(const TextureManager &) = delete;            // no copy-assignment
  TextureManager(TextureManager &&) = delete;            // no move
  TextureManager &operator=(TextureManager &&) = delete; // no move-assignment

public:
  SDL_Texture *GetTexture(const std::string &filePath);

private:
  std::unordered_map<std::string, SDL_Texture *> m_loadedTextures;
  SDL_Renderer *m_SDL_Renderer;
};
