#pragma once

#include <SDL2/SDL.h>
#include "../Engine/TextureManager.h"
#include <memory>

class Game {
  public:
    Game();
    ~Game();
    Game(const Game&) = delete; // no copy
    Game& operator=(const Game&) = delete; // no copy-assignment
    Game(Game&&) = delete; // no move
    Game& operator=(Game&&) = delete; // no move-assignment

  public:
    void StartGame();

  private:
    void ProcessInput();

  private:
    bool m_isInitialized = false;
    bool m_quitGame = false;
    const char* m_title = "MyGame";
    const int m_screenWidth = 800;
    const int m_screenHeight = 600;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Window* m_window = nullptr;
    std::shared_ptr<TextureManager> m_textureMgr = nullptr;
};