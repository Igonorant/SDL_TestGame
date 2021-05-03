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
const std::string Player = "./Assets/character.png";
const std::string PlayerBullet = "./Assets/player_bullet.png";
const std::string Dummy = "./Assets/dummy.png";
} // namespace Assets

} // namespace Global
