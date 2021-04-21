#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;
    game.InitializeSDL();
    game.StartGame();
    return EXIT_SUCCESS;
}

