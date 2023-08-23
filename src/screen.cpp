#include "screen.hpp"

#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <SDL_video.h>

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


SDL_EventType Screen::input() {
    SDL_Event event;

    // Handle events
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            // Close window button or ESC key
            case SDL_QUIT:
                return SDL_QUIT;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return SDL_QUIT;
            
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    std::size_t newWidth = event.window.data1;
                    std::size_t newHeight = event.window.data2;
                    SDL_RenderSetLogicalSize(renderer, newWidth, newHeight);
                }
                else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
                    SDL_MaximizeWindow(window);
                    int w, h;
                    SDL_GetWindowSize(window, &w, &h);
                    SDL_RenderSetLogicalSize(renderer, w, h);
                }
                break;
        }
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
    for (const auto& image : images)
        SDL_RenderCopy(renderer, image.texture, NULL, &image.rect);

    SDL_RenderPresent(renderer);
}


void Screen::putPictureFrame(int x, int y, int w, int h, std::string path) {
    SDL_Surface* pictureSurface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, pictureSurface);
    SDL_FreeSurface(pictureSurface);

    images.push_back( Image {
        SDL_Rect{x, y, w, h}, 
        texture
    });
}


void Screen::clearPictures() {
    for (auto& image : images)
        SDL_DestroyTexture(image.texture);

    images.clear();
}


Screen::~Screen() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}