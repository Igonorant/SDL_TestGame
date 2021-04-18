
#include <SDL2/SDL.h>

#include <iostream>


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
        std::cout << "SDL initialized!" << std::endl;
    } else {
        std::cout << "SDL initialization error" << std::endl;
    }
    system ("pause");
    return EXIT_SUCCESS;
}





