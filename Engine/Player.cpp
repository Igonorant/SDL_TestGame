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
    switch (event) {
    case KbdEvents::Left_KeyDown:
      setVelocityX(-0.001f);
      if (getState() == ObjState::Idle) {
        setState(ObjState::Moving);
      } else if (getState() == ObjState::Firing) {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::Right_KeyDown:
      setVelocityX(0.001f);
      if (getState() == ObjState::Idle) {
        setState(ObjState::Moving);
      } else if (getState() == ObjState::Firing) {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::Space_KeyDown:
      setVelocityY(-0.005f);
      if (getState() == ObjState::Idle) {
        setState(ObjState::Moving);
      } else if (getState() == ObjState::Firing) {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::Up_KeyUp:
    case KbdEvents::Down_KeyUp:
      if (getVelocityX() == 0.0f) {
        if (getState() == ObjState::Moving) {
          setState(ObjState::Idle);
        } else if (getState() == ObjState::FiringAndMoving) {
          setState(ObjState::Firing);
        }
      }
      break;
    case KbdEvents::Left_KeyUp:
    case KbdEvents::Right_KeyUp:
      setVelocityX(0.0f);
      if (getVelocityY() == 0.0f) {
        if (getState() == ObjState::Moving) {
          setState(ObjState::Idle);
        } else if (getState() == ObjState::FiringAndMoving) {
          setState(ObjState::Firing);
        }
      }
      break;
    case KbdEvents::LCtrl_KeyDown:
      if (getVelocityX() == 0.0f && getVelocityY() == 0.0f) {
        setState(ObjState::Firing);
      } else {
        setState(ObjState::FiringAndMoving);
      }
      break;
    case KbdEvents::LCtrl_KeyUp:
      if (getVelocityX() == 0.0f && getVelocityY() == 0.0f) {
        setState(ObjState::Idle);
      } else {
        setState(ObjState::Moving);
      }
      break;
    default:
      break;
    }
  }

  // Update bullet timer
  if (getState() == ObjState::Firing ||
      getState() == ObjState::FiringAndMoving) {
    m_bulletTimer += dt;
  } else {
    m_bulletTimer = 0;
  }

  // Parent class update
  DynamicObject::update(dt);
}

bool Player::shouldSpawnBullet() {
  if ((getState() == ObjState::Firing ||
       getState() == ObjState::FiringAndMoving) &&
      m_bulletTimer >= m_fireRate) {
    m_bulletTimer %= m_fireRate;
    return true;
  }
  return false;
}
// PLAYER END
