#include "Definitions.h"
#include "Game.h"

void Game::initialize() {
  m_textureMgr = std::make_shared<TextureManager>(m_renderer);

  m_modelTimer.setInterval(Uint32(1000 / Global::Game::ModelRate));
  m_frameTimer.setInterval(Uint32(1000 / Global::Game::FrameRate));

  m_player.setDestination({0.1f, 0.5f, 0.1f, 0.1f});
  m_player.setHealth(100);
  m_player.setFireRate(12);

  // Load animations for player
  // TODO: find some assets to add animations to player states
  Animation idle;
  for (unsigned int i = 0; i < 1; ++i) {
    idle.addFrame({
        m_textureMgr->GetTexture(Global::Assets::Player),
        {0, 0, 0, 0},
        100 /*time*/,
        true /*queryTexture*/
    });
  }
  m_player.addAnimation(ObjState::Idle, idle);
  m_player.addAnimation(ObjState::Moving, idle);
  m_player.addAnimation(ObjState::Jumping, idle);
  m_player.addAnimation(ObjState::Firing, idle);
  m_player.addAnimation(ObjState::FiringAndMoving, idle);
  m_player.addAnimation(ObjState::FiringAndJumping, idle);

  // Player Bullet
  m_playerBullet.setDamage(5);
  m_playerBullet.setLifeSpan(1000);
  m_playerBullet.setVelocityX(0.0007f);
  m_playerBullet.setGravitySensitive(false);
  Animation bulletAnim;
  for (unsigned int i = 0; i < 4; ++i) {
    bulletAnim.addFrame({
        m_textureMgr->GetTexture(Global::Assets::PlayerBullet),
        {0, 0, 0, 0},
        100 /*time*/,
        true /*queryTexture*/
    });
  }
  m_playerBullet.setState(ObjState::Moving);
  m_playerBullet.addAnimation(ObjState::Moving, bulletAnim);
  m_playerBullet.scale(0.05f);

  // Test stuff
  m_dummy.setDestination({0.8f, 0.9f, 0.1f, 0.1f});
  m_dummy.setAnimation(Animation(
      {AnimationFrame(m_textureMgr->GetTexture(Global::Assets::Dummy),
                      {} /*src*/, 100 /*ticks*/, true /*queryTexture*/)}));
}
