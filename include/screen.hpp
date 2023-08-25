#pragma once

// C++ standard libraries
#include <cstddef>
#include <string>
#include <vector>

// SDL2 libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


class Screen {
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    Screen(int width, int height, std::string windowName);

    void getSize(int& w, int& h) const;

    void resize(int newWidth, int newHeight);

    void maximize(int& w, int& h);

    SDL_Texture* toTexture(SDL_Surface* surface);

    void putBackground(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t opacity = 255);

    void putTexturedRect(int x, int y, int w, int h, SDL_Texture* texture);

    void putLine(int x1, int y1, int x2, int y2, 
                uint8_t r = 255, uint8_t g= 255, 
                uint8_t b = 255, uint8_t opacity = 255);

    void putRect(int x1, int y1, int x2, int y2, 
                uint8_t r = 255, uint8_t g= 255, 
                uint8_t b = 255, uint8_t opacity = 255);

    void clear();

    void show();

    ~Screen();
};