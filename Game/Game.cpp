#include "Game.h"
#include <iostream>

Game::Game() {

}

Game::~Game() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

void Game::InitializeSDL() {

    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }

    m_window = SDL_CreateWindow(m_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, windowFlags);
    if (!m_window) {
        std::cout << "Couldn't open window: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
    if (!m_renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        std::cout << "Couldn't create renderer: " << SDL_GetError() << std::endl;
        return;
    }

    m_isInitialized = bool(m_window) && bool(m_renderer);
}


void Game::StartGame() {
    m_quitGame |= !m_isInitialized;
    while (!m_quitGame)
    {
        SDL_SetRenderDrawColor(m_renderer, 96, 128, 255, 255);
        SDL_RenderClear(m_renderer);

        ProcessInput();

        SDL_RenderPresent(m_renderer);

        SDL_Delay(16);
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_quitGame = true; break;
            default: break;
        }
    }
}
