#include "Game.h"
#include <iostream>
#include <vector>

Game::Game() {
    int rendererFlags, windowFlags;
    rendererFlags = SDL_RENDERER_ACCELERATED;
    windowFlags = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
    }

    m_window = SDL_CreateWindow(m_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, windowFlags);
    if (!m_window) {
        std::cout << "Couldn't open window: " << SDL_GetError() << std::endl;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
    if (!m_renderer) {
        std::cout << "Couldn't create renderer: " << SDL_GetError() << std::endl;
    } else {
        m_textureMgr = std::make_shared<TextureManager>(m_renderer);
    }

    m_isInitialized = bool(m_window) && bool(m_renderer);
}

Game::~Game() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

void Game::StartGame() {
    m_quitGame |= !m_isInitialized;

    loadAssets();

    struct character {
        SDL_Rect dest;
        SDL_Texture* texture;
    };

    character player;
    player.dest.x = 100;
    player.dest.y = 100;
    player.texture = m_textureMgr->GetTexture("Assets/character.png");
	SDL_QueryTexture(player.texture, nullptr, nullptr, &player.dest.w, &player.dest.h);

    while (!m_quitGame)
    {
        SDL_SetRenderDrawColor(m_renderer, 96, 128, 255, 255);
        SDL_RenderClear(m_renderer);

        ProcessInput();

        SDL_RenderCopy(m_renderer, player.texture, nullptr, &player.dest);
        SDL_RenderPresent(m_renderer);

        SDL_Delay(16);
    }
    return;
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

void Game::loadAssets() {
    std::vector<std::string> assetsToLoad = {
        "Assets/character.png"
    };
    for (const auto& filePath : assetsToLoad) {
        m_textureMgr->GetTexture(filePath);
    }
}

