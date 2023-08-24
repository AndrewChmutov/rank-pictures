#include "screen.hpp"

#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <cstdlib>

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

    font = TTF_OpenFont("fonts/Raleway-Regular.ttf", 400);

    if (font == NULL) {
        fprintf(stderr, "%s\n", "Could not initialize font!");
        exit(EXIT_FAILURE);
    }
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
    
    for (const auto& label : labels)
        SDL_RenderCopy(renderer, label.texture, NULL, &label.rect);

    SDL_RenderPresent(renderer);
}


void Screen::putPictureFrame(int x, int y, int w, int h, std::string path) {
    SDL_Surface* pictureSurface = IMG_Load(path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, pictureSurface);
    SDL_FreeSurface(pictureSurface);

    images.push_back( EntityTexture {
        SDL_Rect{x, y, w, h}, 
        texture
    });
}


void Screen::putLabelFrame(int x, int y, int w, int h, std::string text, 
                            uint8_t r, uint8_t g, uint8_t b, uint8_t opacity) {
    SDL_Surface* labelSurface = TTF_RenderText_Solid(font, text.c_str(), SDL_Color{r, g, b, opacity});
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, labelSurface);
    SDL_FreeSurface(labelSurface);

    labels.push_back( EntityTexture {
        SDL_Rect{x, y, w, h},
        texture
    });
}


void Screen::clear() {
    for (auto& image : images)
        SDL_DestroyTexture(image.texture);

    for (auto& label : labels)
        SDL_DestroyTexture(label.texture);

    images.clear();
}


Screen::~Screen() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}