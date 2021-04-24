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
        loadAssets();
        m_player.setPos(100, 100);
        m_player.setTexture(m_textureMgr->GetTexture("Assets/character.png"));
    }

    m_isInitialized = bool(m_window) && bool(m_renderer);
}

Game::~Game() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

void Game::StartGame() {
    m_quitGame |= !m_isInitialized;

    while (!m_quitGame)
    {
        SDL_SetRenderDrawColor(m_renderer, 96, 128, 255, 255);
        SDL_RenderClear(m_renderer);

        processInput();
        m_player.update();

        m_player.render(m_renderer);
        SDL_RenderPresent(m_renderer);

        SDL_Delay(16);
    }
    return;
}

void Game::processInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_quitGame = true; break;
            case SDL_KEYDOWN:
                processKeydown(&event.key); break;
            case SDL_KEYUP:
                processKeyup(&event.key); break;
            default: break;
        }
    }
}

void Game::processKeydown(SDL_KeyboardEvent* event)
{
    if (event->repeat != 0) {
        return;
    }
    switch (event->keysym.scancode) {
        case SDL_SCANCODE_UP:
            m_player.setVelocityY(-5); break;
        case SDL_SCANCODE_DOWN:
            m_player.setVelocityY(5); break;
        case SDL_SCANCODE_LEFT:
            m_player.setVelocityX(-5); break;
        case SDL_SCANCODE_RIGHT:
            m_player.setVelocityX(5); break;
        default: break;
    }
}

void Game::processKeyup(SDL_KeyboardEvent* event)
{
    if (event->repeat != 0) {
        return;
    }
    switch (event->keysym.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_RIGHT:
            m_player.setVelocity(0, 0); break;
        default: break;
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

