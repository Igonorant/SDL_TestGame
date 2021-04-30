#include "Game.h"
#include <algorithm>
#include <iostream>
#include <vector>

Game::Game() {
  int rendererFlags, windowFlags;
  rendererFlags = SDL_RENDERER_ACCELERATED;
  windowFlags = 0;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
  }

  m_window = SDL_CreateWindow(m_title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, m_screenWidth,
                              m_screenHeight, windowFlags);
  if (!m_window) {
    std::cout << "Couldn't open window: " << SDL_GetError() << std::endl;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
  if (!m_renderer) {
    std::cout << "Couldn't create renderer: " << SDL_GetError() << std::endl;
  } else {
    m_textureMgr = std::make_shared<TextureManager>(m_renderer);
    loadAssets();
    m_player.setPos(100, 100);
    m_player.setTexture(m_textureMgr->GetTexture("Assets/character.png"));
  }

  m_isInitialized = bool(m_window) && bool(m_renderer);
}

Game::~Game() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

void Game::StartGame() {
  m_quitGame |= !m_isInitialized;
  int dt_ms = 16;
  while (!m_quitGame) {
    const auto &events = processInput();
    m_player.update(dt_ms, events);
    for (auto &bullet : m_playerBullets) {
      bullet.update(dt_ms);
    }
    m_playerBullets.erase(std::remove_if(m_playerBullets.begin(),
                                         m_playerBullets.end(),
                                         [](const Projectile &bullet) {
                                           return bullet.endedLifespan();
                                         }),
                          m_playerBullets.end());
    SDL_SetRenderDrawColor(m_renderer, 96, 128, 255, 255);
    SDL_RenderClear(m_renderer);

    m_player.render(m_renderer);
    for (auto &bullet : m_playerBullets) {
      bullet.render(m_renderer);
    }
    SDL_RenderPresent(m_renderer);

    SDL_Delay(dt_ms);
  }
  return;
}

std::vector<KbdEvents> Game::processInput() {
  std::vector<KbdEvents> ret;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT: {
      m_quitGame = true;
      break;
    }
    case SDL_KEYDOWN: {
      const auto &keysDown = processKeydown(&event.key);
      ret.reserve(ret.size() + keysDown.size());
      ret.insert(ret.end(), keysDown.begin(), keysDown.end());
      break;
    }
    case SDL_KEYUP: {
      const auto &keysUp = processKeyup(&event.key);
      ret.reserve(ret.size() + keysUp.size());
      ret.insert(ret.end(), keysUp.begin(), keysUp.end());
      break;
    }
    default: {
      break;
    }
    }
  }
  return ret;
}

std::vector<KbdEvents> Game::processKeydown(SDL_KeyboardEvent *event) {
  std::vector<KbdEvents> ret;
  if (event->repeat != 0) {
    return ret;
  }
  switch (event->keysym.scancode) {
  case SDL_SCANCODE_UP:
    ret.push_back(KbdEvents::Up_KeyDown);
    break;
  case SDL_SCANCODE_DOWN:
    ret.push_back(KbdEvents::Down_KeyDown);
    break;
  case SDL_SCANCODE_LEFT:
    ret.push_back(KbdEvents::Left_KeyDown);
    break;
  case SDL_SCANCODE_RIGHT:
    ret.push_back(KbdEvents::Right_KeyDown);
    break;
  case SDL_SCANCODE_LCTRL:
    ret.push_back(KbdEvents::LCtrl_KeyDown);
    m_playerBullets.emplace_back(
        m_textureMgr->GetTexture("Assets/player_bullet.png"),
        m_player.getPosX() + 15, m_player.getPosY() + 20, 1 /*vx*/,
        m_player.getVelocityY(), 500 /*lifespan_ms*/, 10 /*damage*/);
    m_playerBullets.back().scale(0.025f);
    break;
  default:
    break;
  }
  return ret;
}

std::vector<KbdEvents> Game::processKeyup(SDL_KeyboardEvent *event) {
  std::vector<KbdEvents> ret;
  if (event->repeat != 0) {
    return ret;
  }
  switch (event->keysym.scancode) {
  case SDL_SCANCODE_UP:
    ret.push_back(KbdEvents::Up_KeyUp);
    break;
  case SDL_SCANCODE_DOWN:
    ret.push_back(KbdEvents::Down_KeyUp);
    break;
  case SDL_SCANCODE_LEFT:
    ret.push_back(KbdEvents::Left_KeyUp);
    break;
  case SDL_SCANCODE_RIGHT:
    ret.push_back(KbdEvents::Right_KeyUp);
    break;
  default:
    break;
  }
  return ret;
}

void Game::loadAssets() {
  std::vector<std::string> assetsToLoad = {"Assets/character.png",
                                           "Assets/player_bullet.png"};
  for (const auto &filePath : assetsToLoad) {
    m_textureMgr->GetTexture(filePath);
  }
}
