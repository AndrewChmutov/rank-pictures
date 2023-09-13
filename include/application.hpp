#pragma once

// Custom libraries
#include "screen.hpp"
#include "menu_events.hpp"
#include "base_menu.hpp"
#include "picture_record.hpp"
#include "data_handler.hpp"

// C++ standard libraries
#include <filesystem>
#include <vector>
#include <memory>

// Application class
// The main features:
// - Rank pictures
// - View the results
class Application {
    // Screen for showing pictures
    Screen screen;
    DataHandler dataHandler;

    // Pictures information
    std::vector<PictureRecord> pictures;
    std::string pathToPictures;

    // Font position
    std::string pathToFont;

    // Flag for main loop
    bool isRunning;

    // Handles menu logic of the app 
    std::unique_ptr<BaseMenu> currentMenu;


    // Change view to main menu:
    // Compare pictures and count the choices
    void switchToMain();

    // Change view to rank menu:
    // See the counters and pictures in order
    void switchToRank(MenuEvent event);


    void handleEvents();

    // Update current menu
    void update();

    // Render current menu
    void render();

    // Debug information into ostream
    void debug() const;

    // Checks if given directory entry is picture
    bool isPicture(const std::filesystem::directory_entry& entry) const;
public:
    Application(std::size_t w, std::size_t h, const std::string& pathToPictures, 
                    const std::string& pathToFont, std::string pathToBackground = "");

    // Application main loop
    int run();

    ~Application();
};