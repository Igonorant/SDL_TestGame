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
  LCtrl_KeyUp
};

enum class ObjState { Firing, Moving, Idle };

class Frame;

class AnimationFrame;

class Animation;

class Object;

class Player;

class Projectile;

class Time;
