#pragma once

// C++ standard libraries
#include <SDL_render.h>
#include <string>
#include <vector>

// SDL2 libraries
#include <SDL2/SDL.h>


class Screen {
    SDL_Window* window;
    SDL_Renderer* renderer;

    struct Image {
        SDL_Rect rect;
        SDL_Texture* texture;
    };

    std::vector<Image> images;

public:
    Screen(std::size_t width, std::size_t height);

    SDL_EventType input();

    void putBackground(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t opacity = 255);

    void putPictureFrame(int x, int y, int h, int w, std::string path);

    void clearPictures();

    void show();

    ~Screen();
};