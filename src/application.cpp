#include "application.hpp"

// Custom libraries
#include "screen.hpp"
#include "menu_events.hpp"
#include "main_menu.hpp"
#include "rank_menu.hpp"
#include "picture_record.hpp"

// C++ standard libraries
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>


Application::Application(std::size_t w, std::size_t h, const std::string& pathToPictures, const std::string& pathToFont, std::string pathToBackground) :
        // Setup a screen
        screen(w, h, "Picture ranking"),

        // Setup main paths
        pathToPictures(pathToPictures),
        pathToFont(pathToFont),
        dataHandler(pathToPictures) {
    
    // Get all the current pictures in the directory //

    pictures.clear();
    for(auto& entry : std::filesystem::directory_iterator(pathToPictures)) {
        // If the picture is met - write the record
        if (isPicture(entry)) {
            pictures.push_back(PictureRecord{
                entry.path().filename().string(), 
                0,
                0,
                entry.path().string()
            });
        }

    }

    dataHandler.getData(pictures);

    // Background 
    if (pathToBackground != "")
        screen.setBackground(pathToBackground);

    // Start from main menu
    switchToMain();
}


int Application::run() {
    // Flag for main loop
    isRunning = true;

    // For establishing fps limit
    float desiredDelta = 1.0f / 60;

    // Main loop
    while (isRunning) {
        // Start timer
        auto start = std::chrono::high_resolution_clock::now();

        // Main structure of the app
        handleEvents();
        if (currentMenu->toUpdate()) {
            update();
            render();
        }

        // Capture the time
        auto finish = std::chrono::high_resolution_clock::now();

        // Calculate the time to sleep
        std::chrono::duration<float> elapsed = finish - start;
        if (elapsed.count() < desiredDelta) {
            std::this_thread::sleep_for(std::chrono::duration<float>(desiredDelta - elapsed.count()));
        }
    }

    return 0;
}


Application::~Application() {
    // debug();
    dataHandler.updateData(pictures);
}


void Application::switchToMain() {
    // Change view to Main menu
    currentMenu = std::make_unique<MainMenu>(
        screen,
        pictures,
        pathToFont
    );
}


void Application::switchToRank(MenuEvent event) {
    // Sort picture records
    std::sort(pictures.begin(), pictures.end(),
        [](const PictureRecord& first, const PictureRecord& second) {
            return first.wins > second.wins;
        }
    );

    // Switch the view to Rank menu
    currentMenu = std::make_unique<RankMenu>(
        screen,
        pictures,
        pathToFont
    );
}


void Application::handleEvents() {
    // Hangle SDL events
    MenuEvent event = currentMenu.get()->handleEvents(screen);

    // Communication between menu and app
    switch (event) {
        // Exit command
        case MenuEvent::EXIT:
            isRunning = false;
            break;
        
        // Main screen command
        case MenuEvent::TO_MAIN_SCREEN:
            switchToMain();
            break;

        // Rating screen command
        case MenuEvent::TO_RATING_SCREEN:
            switchToRank(event);
            break;

        // If no info is provided
        case MenuEvent::NONE:
            break;
    }
}


void Application::update() {
    // Update window elements
    currentMenu.get()->update(screen);
}


void Application::render() {
    // Render window elements
    currentMenu.get()->render(screen);
}


void Application::debug() const {
    // Show information of each picture record
    for(auto& picture : pictures) {
        std::cout << "Name: " << picture.name << std::endl;
        std::cout << "Wins: " << picture.wins << std::endl;
        std::cout << "Total: " << picture.total << std::endl;
        std::cout << "Success rate: " << 1.0f * picture.wins / picture.total << std::endl << std::endl;
    }
}


bool Application::isPicture(const std::filesystem::directory_entry& entry) const {
    return (
        entry.path().extension().string() == ".jpg" ||
        entry.path().extension().string() == ".jpeg" ||
        entry.path().extension().string() == ".png" ||
        entry.path().extension().string() == ".bmp"
    );
}