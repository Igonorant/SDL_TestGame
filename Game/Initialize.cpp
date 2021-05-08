#include "Definitions.h"
#include "Game.h"

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
        m_textureMgr->GetTexture(Global::Assets::Player),
        {20, 20, 500, 500},
        100 /*time*/
    });
  }
  m_player.addAnimation(Player::AnimationState::Idle, idle);

  Animation moving;
  for (unsigned int i = 0; i < 8; ++i) {
    moving.addFrame({
        m_textureMgr->GetTexture(Global::Assets::Player),
        {20, 20, 500, 500},
        100 /*time*/
    });
  }
  m_player.addAnimation(Player::AnimationState::Moving, moving);

  Animation firingAndMoving;
  for (unsigned int i = 0; i < 9; ++i) {
    firingAndMoving.addFrame({
        m_textureMgr->GetTexture(Global::Assets::Player),
        {20, 20, 500, 500},
        100 /*time*/
    });
  }
  m_player.addAnimation(Player::AnimationState::FiringAndMoving,
                        firingAndMoving);

  Animation firing;
  for (unsigned int i = 0; i < 4; ++i) {
    firing.addFrame({
        m_textureMgr->GetTexture(Global::Assets::Player),
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
