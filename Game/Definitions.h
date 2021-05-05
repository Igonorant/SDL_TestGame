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
const std::string PlayerIdle01 = "./Assets/Player/Idle (1).png";
const std::string PlayerIdle02 = "./Assets/Player/Idle (2).png";
const std::string PlayerIdle03 = "./Assets/Player/Idle (3).png";
const std::string PlayerIdle04 = "./Assets/Player/Idle (4).png";
const std::string PlayerIdle05 = "./Assets/Player/Idle (5).png";
const std::string PlayerIdle06 = "./Assets/Player/Idle (6).png";
const std::string PlayerIdle07 = "./Assets/Player/Idle (7).png";
const std::string PlayerIdle08 = "./Assets/Player/Idle (8).png";
const std::string PlayerIdle09 = "./Assets/Player/Idle (9).png";
const std::string PlayerIdle10 = "./Assets/Player/Idle (10).png";

const std::string Player = "./Assets/Tmp/character.png";
const std::string PlayerBullet = "./Assets/Tmp/player_bullet.png";
const std::string Dummy = "./Assets/Tmp/dummy.png";
} // namespace Assets

} // namespace Global
