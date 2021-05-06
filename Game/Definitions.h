#pragma once

#include <string>

namespace Global {

namespace Game {
const std::string Name = "MyGame";
const int FrameRate = 60;  // frames per second
const int ModelRate = 200; // updates per second
} // namespace Game

namespace SDL {
const int ScreenWidth = 800;
const int ScreenHeight = 600;
} // namespace SDL

namespace Assets {
const std::string getPlayerIdle(const unsigned int texture) {
  switch (texture % 10) {
  case 0:
    return "./Assets/Player/Idle (1).png";
  case 1:
    return "./Assets/Player/Idle (2).png";
  case 2:
    return "./Assets/Player/Idle (3).png";
  case 3:
    return "./Assets/Player/Idle (4).png";
  case 4:
    return "./Assets/Player/Idle (5).png";
  case 5:
    return "./Assets/Player/Idle (6).png";
  case 6:
    return "./Assets/Player/Idle (7).png";
  case 7:
    return "./Assets/Player/Idle (8).png";
  case 8:
    return "./Assets/Player/Idle (9).png";
  case 9:
    return "./Assets/Player/Idle (10).png";
  }
}

const std::string getPlayerMoving(const unsigned int texture) {
  switch (texture % 8) {
  case 0:
    return "./Assets/Player/Run (1).png";
  case 1:
    return "./Assets/Player/Run (2).png";
  case 2:
    return "./Assets/Player/Run (3).png";
  case 3:
    return "./Assets/Player/Run (4).png";
  case 4:
    return "./Assets/Player/Run (5).png";
  case 5:
    return "./Assets/Player/Run (6).png";
  case 6:
    return "./Assets/Player/Run (7).png";
  case 7:
    return "./Assets/Player/Run (8).png";
  }
}

const std::string getPlayerFiringAndMoving(const unsigned int texture) {
  switch (texture % 9) {
  case 0:
    return "./Assets/Player/RunShoot (1).png";
  case 1:
    return "./Assets/Player/RunShoot (2).png";
  case 2:
    return "./Assets/Player/RunShoot (3).png";
  case 3:
    return "./Assets/Player/RunShoot (4).png";
  case 4:
    return "./Assets/Player/RunShoot (5).png";
  case 5:
    return "./Assets/Player/RunShoot (6).png";
  case 6:
    return "./Assets/Player/RunShoot (7).png";
  case 7:
    return "./Assets/Player/RunShoot (8).png";
  case 8:
    return "./Assets/Player/RunShoot (9).png";
  }
}

const std::string getPlayerFiring(const unsigned int texture) {
  switch (texture % 4) {
  case 0:
    return "./Assets/Player/Shoot (1).png";
  case 1:
    return "./Assets/Player/Shoot (2).png";
  case 2:
    return "./Assets/Player/Shoot (3).png";
  case 3:
    return "./Assets/Player/Shoot (4).png";
  }
}

const std::string Player = "./Assets/Tmp/character.png";
const std::string PlayerBullet = "./Assets/Tmp/player_bullet.png";
const std::string Dummy = "./Assets/Tmp/dummy.png";
} // namespace Assets

} // namespace Global
