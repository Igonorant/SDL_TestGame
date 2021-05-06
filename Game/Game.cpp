#include "Game.h"
#include "Definitions.h"
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

  m_window =
      SDL_CreateWindow(Global::Game::Name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, Global::SDL::ScreenWidth,
                       Global::SDL::ScreenHeight, windowFlags);
  if (!m_window) {
    std::cout << "Couldn't open window: " << SDL_GetError() << std::endl;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
  if (m_renderer) {
    initialize();
  } else {
    std::cout << "Couldn't create renderer: " << SDL_GetError() << std::endl;
  }

  m_isInitialized = bool(m_window) && bool(m_renderer);
}

Game::~Game() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

void Game::StartGame() {
  m_quitGame |= !m_isInitialized;
  while (!m_quitGame) {
    updateModel();
    if (m_frameTimer.triggered()) {
      composeFrame();
    }

    m_modelTimer.waitUntilNextTrigger();
  }
  return;
}

void Game::initialize() {
  m_textureMgr = std::make_shared<TextureManager>(m_renderer);

  m_modelTimer.setInterval(Uint32(1000 / Global::Game::ModelRate));
  m_frameTimer.setInterval(Uint32(1000 / Global::Game::FrameRate));

  m_player.initialize({nullptr,
                       1.0f /*scale*/,
                       {100, 250, 120, 180} /*pos*/,
                       false /*queryTexture*/,
                       0.0f /*vx*/,
                       0.0f /*vy*/,
                       {ObjState::Idle}},
                      100 /*health*/, 5 /*fireRate*/);

  // Load animations for player
  Animation idle;
  for (unsigned int i = 0; i < 10; ++i) {
    idle.addFrame({
        m_textureMgr->GetTexture(Global::Assets::getPlayerIdle(i)),
        {20, 20, 500, 500},
        100 /*time*/
    });
  }
  m_player.addAnimation(Player::AnimationState::Idle, idle);

  Animation moving;
  for (unsigned int i = 0; i < 8; ++i) {
    moving.addFrame({
        m_textureMgr->GetTexture(Global::Assets::getPlayerMoving(i)),
        {20, 20, 500, 500},
        100 /*time*/
    });
  }
  m_player.addAnimation(Player::AnimationState::Moving, moving);

  Animation firingAndMoving;
  for (unsigned int i = 0; i < 9; ++i) {
    firingAndMoving.addFrame({
        m_textureMgr->GetTexture(Global::Assets::getPlayerFiringAndMoving(i)),
        {20, 20, 500, 500},
        100 /*time*/
    });
  }
  m_player.addAnimation(Player::AnimationState::FiringAndMoving,
                        firingAndMoving);

  Animation firing;
  for (unsigned int i = 0; i < 4; ++i) {
    firing.addFrame({
        m_textureMgr->GetTexture(Global::Assets::getPlayerFiring(i)),
        {20, 20, 500, 500},
        100 /*time*/
    });
  }
  m_player.addAnimation(Player::AnimationState::Firing, firing);

  // Test stuff
  m_dummy.initialize({m_textureMgr->GetTexture(Global::Assets::Dummy),
                      1.0f /*scale*/,
                      {500, 250} /*pos*/,
                      true /*queryTexture*/,
                      0.0f /*vx*/,
                      0.0f /*vy*/,
                      {ObjState::Idle}});
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
  case SDL_SCANCODE_LCTRL:
    ret.push_back(KbdEvents::LCtrl_KeyUp);
    break;
  default:
    break;
  }
  return ret;
}

void Game::updateModel() {
  // Get time since last loop
  const Uint32 dt = m_modelTimer.getTimeSinceLastCall();

  // Get events
  const auto &events = processInput();

  // Update player
  m_player.update(dt, events);

  // Spawn bullets
  if (m_player.shouldSpawnBullet()) {
    m_playerBullets.emplace_back(
        Object::InitList{m_textureMgr->GetTexture(Global::Assets::PlayerBullet),
                         0.025f /*scale*/,
                         {int(m_player.getPosX()) + 15,
                          int(m_player.getPosY()) + 20} /*pos*/,
                         true /*queryTexture*/,
                         0.5f /*vx*/,
                         m_player.getVelocityY() /*vy*/,
                         {ObjState::Moving}},
        1000 /*lifespan_ms*/, 10 /*damage*/);
  }

  // Update bullets
  for (auto &bullet : m_playerBullets) {
    bullet.update(dt);
    if (m_dummy.isColiding(bullet)) {
      bullet.hitted();
    }
  }

  // Remove dying bullets
  m_playerBullets.erase(std::remove_if(m_playerBullets.begin(),
                                       m_playerBullets.end(),
                                       [](const Projectile &bullet) {
                                         return bullet.endedLifespan();
                                       }),
                        m_playerBullets.end());

  // Test stuff
  m_testAnimation.update(dt);
}

void Game::composeFrame() {
  // Render background
  SDL_SetRenderDrawColor(m_renderer, 96, 128, 255, 255);
  SDL_RenderClear(m_renderer);

  // Render objects
  m_player.render(m_renderer);
  m_dummy.render(m_renderer);
  for (auto &bullet : m_playerBullets) {
    bullet.render(m_renderer);
  }

  // Test stuff
  m_testAnimation.render(m_renderer, {100, 100, 120, 150});

  // Present rendered objects
  SDL_RenderPresent(m_renderer);
}
