#pragma once

enum class KbdEvents {
  Up_KeyDown,
  Up_KeyUp,
  Down_KeyDown,
  Down_KeyUp,
  Left_KeyDown,
  Left_KeyUp,
  Right_KeyDown,
  Right_KeyUp,
  LCtrl_KeyDown,
  LCtrl_KeyUp,
  Space_KeyDown,
  Space_KeyUp,
};

enum class ObjState {
  Idle,
  Moving,
  Jumping,
  Firing,
  FiringAndMoving,
  FiringAndJumping
};

struct Rectf {
  float x, y, w, h;
};

class Frame;

class AnimationFrame;

class Animation;

class Object;

class DynamicObject;

class Player;

class Projectile;

class Time;
