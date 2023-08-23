#pragma once

// SDL2 libraries
#include <SDL2/SDL.h>


class Screen {
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    Screen(std::size_t width, std::size_t height);

    SDL_EventType input();

    void putBackground(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t opacity = 255);

    void show();

    ~Screen();
};