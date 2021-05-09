#pragma once

#include "../Engine/Components.h"
#include "../Engine/Components_forward.h"
#include "../Engine/TextureManager.h"
#include <SDL2/SDL.h>
#include <memory>
#include <vector>

class Game {
public:
  Game();
  ~Game();
  Game(const Game &) = delete;            // no copy
  Game &operator=(const Game &) = delete; // no copy-assignment
  Game(Game &&) = delete;                 // no move
  Game &operator=(Game &&) = delete;      // no move-assignment

public:
  void StartGame();

private:
  void initialize();

  std::vector<KbdEvents> processInput();
  std::vector<KbdEvents> processKeydown(SDL_KeyboardEvent *event);
  std::vector<KbdEvents> processKeyup(SDL_KeyboardEvent *event);

  void updateModel();
  void composeFrame();

private:
  // General
  bool m_isInitialized = false;
  bool m_quitGame = false;
  SDL_Renderer *m_renderer = nullptr;
  SDL_Window *m_window = nullptr;
  std::shared_ptr<TextureManager> m_textureMgr = nullptr;

  // Player objects
  Player m_player;
  Projectile m_playerBullet;
  std::vector<Projectile> m_playerBullets;

  // Timers
  Timer m_modelTimer;
  Timer m_frameTimer;

  // Testing
  Object m_dummy;
  Animation m_testAnimation;
};
