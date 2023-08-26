#pragma once

#include "screen.hpp"
#include "base_menu.hpp"
#include <memory>
#include <random>

class Application {
    // Screen for showing pictures
    Screen screen;

    // paths
    std::string pathToPictures;
    std::string pathToFont;

    // For main loop
    bool isRunning;

    // For handling menu logic 
    std::unique_ptr<BaseMenu> currentMenu;
    
    // Random integer generation
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;

    // Update menu
    void update();

    // Render
    void render();

    // Change view to main menu
    void switchToMain();
public:
    Application(std::size_t w, std::size_t h, std::string pathToPictures, std::string pathToFont);

    // Method for main loop of the application
    int run();

};