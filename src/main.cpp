// C++ standard libraries
#include <iostream>

// Custom libraries
#include "screen.hpp"


int main() {
    Screen screen(1280, 720);
    // ./cars/002b243a2b98c0012415693225b8efbf.jpg
    screen.putPictureFrame(0, 0, 720, 720, "kek.jpg");

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