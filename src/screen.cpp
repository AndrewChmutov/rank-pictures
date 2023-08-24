#include "screen.hpp"

#include <SDL2/SDL_image.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <cstddef>
#include <cstdlib>

Screen::Screen(int width, int height, std::string windowName) {
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

    if (TTF_Init() == -1) {
        fprintf(stderr, "%s\n", "Could not initialize TTF!");
        exit(EXIT_FAILURE);
    }

    // Initialize window and renderer
    window = SDL_CreateWindow(
        "Picture ranking", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        width, 
        height, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    renderer = SDL_CreateRenderer(
        window, 
        -1,
        SDL_RENDERER_ACCELERATED
    );
}


SDL_Texture* Screen::toTexture(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(renderer, surface);
}


void Screen::getSize(int &w, int &h) const {
    SDL_GetWindowSize(window, &w, &h);
}


void Screen::resize(int newWidth, int newHeight) {
    SDL_RenderSetLogicalSize(renderer, newWidth, newHeight);
}


void Screen::maximize(int& w, int& h) {
    SDL_MaximizeWindow(window);
    SDL_GetWindowSize(window, &w, &h);
    SDL_RenderSetLogicalSize(renderer, w, h);
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


void Screen::putTexturedRect(int x, int y, int w, int h, SDL_Texture* texture) {
    SDL_Rect rect {x, y, w, h};
    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        &rect
    );
}


void Screen::putLine(int x1, int y1, int x2, int y2, 
                uint8_t r, uint8_t g, 
                uint8_t b, uint8_t opacity) {
    SDL_SetRenderDrawColor(renderer, r, g, b, opacity);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}


Screen::~Screen() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}