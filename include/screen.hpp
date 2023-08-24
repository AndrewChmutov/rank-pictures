#pragma once

// C++ standard libraries
#include <string>
#include <vector>

// SDL2 libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


class Screen {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    struct EntityTexture {
        SDL_Rect rect;
        SDL_Texture* texture;
    };

    std::vector<EntityTexture> images;
    std::vector<EntityTexture> labels;

public:
    Screen(std::size_t width, std::size_t height);

    SDL_EventType input();

    void putBackground(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t opacity = 255);

    void putPictureFrame(int x, int y, int w, int h, std::string path);

    void putLabelFrame(int x, int y, int w, int h, std::string text,
                        uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t opacity = 255);

    void clear();

    void show();

    ~Screen();
};