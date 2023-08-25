#pragma once

// C++ standard libraries
#include <string>
#include <vector>

// SDL2 libraries
#include <SDL2/SDL.h>


class Screen {
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    Screen(int width, int height, std::string windowName);

    // Get width and hight by reference
    void getSize(int& w, int& h) const;

    // Resizes window by given parameters
    void resize(int newWidth, int newHeight);

    // Maximize the window and return its size
    // after maximizing by reference
    void maximize(int& w, int& h);

    // Get texture. Wrapper for SDL function
    SDL_Texture* toTexture(SDL_Surface* surface);

    // Set the background
    // Usually at the beginning of frame rendering
    void putBackground(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t opacity = 255);

    // Into given rectangle, put the given texture
    void putTexturedRect(int x, int y, int w, int h, SDL_Texture* texture);

    // Plot the line
    void putLine(int x1, int y1, int x2, int y2, 
                uint8_t r = 255, uint8_t g= 255, 
                uint8_t b = 255, uint8_t opacity = 255);

    // Plot the unfilled rectangle
    void putRect(int x1, int y1, int x2, int y2, 
                uint8_t r = 255, uint8_t g= 255, 
                uint8_t b = 255, uint8_t opacity = 255);

    // Finally show the changed frame
    void show();

    ~Screen();
};