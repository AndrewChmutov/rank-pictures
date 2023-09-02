#pragma once

// Custom libraries
#include "menu_events.hpp"
#include "screen.hpp"
#include "base_menu.hpp"
#include "picture_record.hpp"

// C++ standard libraries
#include <vector>
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
    // std::mt19937 gen;
    // std::uniform_int_distribution<int> dist;

    std::vector<PictureRecord> pictures;

    int lastLeft, lastRight;

    // Update menu
    void update();

    // Render
    void render();

    // Change view to main menu
    void switchToMain();


    void switchToRank(MenuEvent event);

    // Debug information into ostream
    void debug();
public:
    Application(std::size_t w, std::size_t h, std::string pathToPictures, std::string pathToFont);

    // Method for main loop of the application
    int run();

    ~Application();
};