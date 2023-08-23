#include "screen.hpp"

#include <SDL2/SDL_image.h>

Screen::Screen(std::size_t width, std::size_t height) {
    // Initialize different subsystems
    if (SDL_Init(SDL_INIT_VIDEO < 0)) {
        fprintf(stderr, "%s\n", "Could not initialize video!");
        exit(EXIT_FAILURE);
    }

    if (!IMG_Init(IMG_INIT_PNG)) {
        fprintf(stderr, "%s\n", "Could not initialize PNG!");
        exit(EXIT_FAILURE);
    }

    if (!IMG_Init(IMG_INIT_JPG)) {
        fprintf(stderr, "%s\n", "Could not initialize JPG!");
        exit(EXIT_FAILURE);
    }

    // Initialize window and renderer
    SDL_CreateWindowAndRenderer(
        width, 
        height, 
        SDL_WINDOW_OPENGL, 
        &window,
        &renderer
    );

    SDL_SetWindowTitle(window, "Picture ranking");
}


SDL_EventType Screen::input() {
    SDL_Event event;

    // Handle events
    while(SDL_PollEvent(&event)) {
        // Close window button or ESC key
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
            return SDL_QUIT;
    }

    // Default case
    return SDL_USEREVENT;
}


void Screen::putBackground(uint8_t r, uint8_t g, uint8_t b, uint8_t opacity) {
    // Color for background
    SDL_SetRenderDrawColor(renderer, r, g, b, opacity);

    // Render background
    SDL_RenderClear(renderer);
}


void Screen::show() {
    SDL_RenderPresent(renderer);
}


Screen::~Screen() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}