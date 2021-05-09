#include "../Game/Definitions.h"
#include "Components.h"

// PLAYER BEGIN
Player::Player(const std::unordered_map<ObjState, Animation> &animations,
               const Rectf &destination, const int health,
               const Uint32 &fireRate, const float vx, const float vy,
               const bool gravitySensitive, const float scale,
               const ObjState state)
    : DynamicObject(animations, destination, vx, vy, gravitySensitive, scale,
                    state),
      m_health(health), m_fireRate(1000 / fireRate) {}

void Player::update(const Uint32 &dt, const std::vector<KbdEvents> &events) {
  // Handle events
  const auto previousState = getState();
  for (const auto event : events) {
    switch (getState()) {
    case ObjState::Idle:
      handleIdleState(event);
      break;
    case ObjState::Moving:
      handleMovingState(event);
      break;
    case ObjState::Jumping:
      handleJumpingState(event);
      break;
    case ObjState::Firing:
      handleFiringState(event);
      break;
    case ObjState::FiringAndMoving:
      handleFiringAndMovingState(event);
      break;
    case ObjState::FiringAndJumping:
      handleFiringAndJumpingState(event);
      break;
    default:
      break;
    }
  }

  // Update bullet timer
  if (isFiring()) {
    m_bulletTimer += dt;
  } else {
    m_bulletTimer = 0;
  }

  // Parent class update
  DynamicObject::update(dt);
}

bool Player::shouldSpawnBullet() {
  if (isFiring() && (m_bulletTimer >= m_fireRate || m_bulletTimer == 0)) {
    m_bulletTimer %= m_fireRate;
    return true;
  }
  return false;
}

bool Player::isFiring() {
  return getState() == ObjState::Firing ||
         getState() == ObjState::FiringAndMoving ||
         getState() == ObjState::FiringAndJumping;
}

void Player::handleIdleState(const KbdEvents event) {
  switch (event) {
  case KbdEvents::Left_KeyDown:
  case KbdEvents::Right_KeyDown: {
    setState(ObjState::Moving);
    const float vx = event == KbdEvents::Left_KeyDown ? -0.001f : 0.001f;
    setVelocityX(vx);
    break;
  }
  case KbdEvents::Space_KeyDown: {
    setState(ObjState::Jumping);
    setVelocityY(-0.005f);
    break;
  }
  case KbdEvents::LCtrl_KeyDown: {
    setState(ObjState::Firing);
    m_bulletTimer = 0;
  }
  default:
    break;
  }
}

void Player::handleMovingState(const KbdEvents event) {
  switch (event) {
  case KbdEvents::Left_KeyDown:
  case KbdEvents::Right_KeyDown: {
    setState(ObjState::Moving);
    const float vx = event == KbdEvents::Left_KeyDown ? -0.001f : 0.001f;
    setVelocityX(vx);
    break;
  }
  case KbdEvents::Left_KeyUp:
  case KbdEvents::Right_KeyUp: {
    setState(ObjState::Idle);
    setVelocityX(0.0f);
    break;
  }
  case KbdEvents::Space_KeyDown: {
    setState(ObjState::Jumping);
    setVelocityY(-0.005f);
    break;
  }
  case KbdEvents::LCtrl_KeyDown: {
    setState(ObjState::FiringAndMoving);
  }
  default:
    break;
  }
}

void Player::handleJumpingState(const KbdEvents event) {
  switch (event) {
  case KbdEvents::Left_KeyDown:
  case KbdEvents::Right_KeyDown: {
    const float vx = event == KbdEvents::Left_KeyDown ? -0.001f : 0.001f;
    setVelocityX(vx);
    break;
  }
  case KbdEvents::Left_KeyUp:
  case KbdEvents::Right_KeyUp: {
    setVelocityX(0.0f);
    break;
  }
    // TODO: enable double jump later
    //   case KbdEvents::Space_KeyDown: {
    //     setState(ObjState::Jumping);
    //     setVelocityY(-0.005f);
    //     break;
    //   }
  case KbdEvents::LCtrl_KeyDown: {
    setState(ObjState::FiringAndJumping);
  }
  default:
    break;
  }
  if (getOppositeY() == Global::Game::Floor) {
    if (getVelocityX() == 0) {
      setState(ObjState::Idle);
    } else {
      setState(ObjState::Moving);
    }
  }
}

void Player::handleFiringState(const KbdEvents event) {
  switch (event) {
  case KbdEvents::Left_KeyDown:
  case KbdEvents::Right_KeyDown: {
    setState(ObjState::FiringAndMoving);
    const float vx = event == KbdEvents::Left_KeyDown ? -0.001f : 0.001f;
    setVelocityX(vx);
    break;
  }
  case KbdEvents::Space_KeyDown: {
    setState(ObjState::FiringAndJumping);
    setVelocityY(-0.005f);
    break;
  }
  case KbdEvents::LCtrl_KeyUp: {
    setState(ObjState::Idle);
  }
  default:
    break;
  }
}

void Player::handleFiringAndMovingState(const KbdEvents event) {
  switch (event) {
  case KbdEvents::Left_KeyDown:
  case KbdEvents::Right_KeyDown: {
    setState(ObjState::FiringAndMoving);
    const float vx = event == KbdEvents::Left_KeyDown ? -0.001f : 0.001f;
    setVelocityX(vx);
    break;
  }
  case KbdEvents::Left_KeyUp:
  case KbdEvents::Right_KeyUp: {
    setState(ObjState::Firing);
    setVelocityX(0.0f);
    break;
  }
  case KbdEvents::Space_KeyDown: {
    setState(ObjState::FiringAndJumping);
    setVelocityY(-0.005f);
    break;
  }
  case KbdEvents::LCtrl_KeyUp: {
    setState(ObjState::Moving);
  }
  default:
    break;
  }
}

void Player::handleFiringAndJumpingState(const KbdEvents event) {
  switch (event) {
  case KbdEvents::Left_KeyDown:
  case KbdEvents::Right_KeyDown: {
    const float vx = event == KbdEvents::Left_KeyDown ? -0.001f : 0.001f;
    setVelocityX(vx);
    break;
  }
  case KbdEvents::Left_KeyUp:
  case KbdEvents::Right_KeyUp: {
    setVelocityX(0.0f);
    break;
  }
    // TODO: enable double jump later
    //   case KbdEvents::Space_KeyDown: {
    //     setState(ObjState::Jumping);
    //     setVelocityY(-0.005f);
    //     break;
    //   }
  case KbdEvents::LCtrl_KeyUp: {
    setState(ObjState::Jumping);
  }
  default:
    break;
  }
  if (getOppositeY() == Global::Game::Floor) {
    if (getVelocityX() == 0) {
      setState(ObjState::Firing);
    } else {
      setState(ObjState::FiringAndMoving);
    }
  }
}

// PLAYER END
