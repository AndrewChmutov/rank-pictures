// C++ standard libraries
#include <iostream>

// Custom libraries
#include "screen.hpp"


int main() {
    Screen screen(1280, 720);

    while (true) {
        SDL_EventType input_result = screen.input();
        screen.putBackground();
        screen.show();

        if (input_result == SDL_QUIT)
            break;

        SDL_Delay(20);
    }

    return 0;
}