#pragma once

#include <string>

namespace Global {

namespace Game {
const std::string Name = "MyGame";
const int FrameRate = 60;  // frames per second
const int ModelRate = 200; // updates per second
const float Floor = 1.0f;  // position of floor
const float Gravity = 0.0001f;
} // namespace Game

namespace SDL {
const int ScreenWidth = 800;
const int ScreenHeight = 600;
} // namespace SDL

namespace Assets {
const std::string Player = "./Assets/Tmp/character.png";
const std::string PlayerBullet = "./Assets/Tmp/player_bullet.png";
const std::string Dummy = "./Assets/Tmp/dummy.png";
} // namespace Assets

} // namespace Global
