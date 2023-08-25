#include "application.hpp"
#include "menu_events.hpp"
#include "main_menu.hpp"
#include <filesystem>


Application::Application(std::size_t w, std::size_t h, std::string pathToPictures, std::string pathToFont) :
    // Setup a screen
    screen(w, h, "Picture ranking"),

    // Setup main paths
    pathToPictures(pathToPictures),
    pathToFont(pathToFont),

    // Setup random generation
    gen(std::random_device()()){

    // Start from main menu
    switchToMain();
}


int Application::run() {
    isRunning = true;

    while (isRunning) {
        update();
    }

    return 0;
}


void Application::update() {
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

        // Rating screen command
        case MenuEvent::TO_RATING_SCREEN:
            // TO-DO
        
        // If no info is provided
        case MenuEvent::NONE:
            break;
    }

    // Update window and render
    currentMenu.get()->update(screen);
    currentMenu.get()->render(screen);
}


void Application::switchToMain() {
    std::size_t countEntries = 0;
    for(auto& entry : std::filesystem::directory_iterator(pathToPictures)) {
        countEntries++;
    }

    std::size_t first, second;
    dist = std::uniform_int_distribution<>(0, countEntries - 1);
    first = dist(gen);

    dist = std::uniform_int_distribution<>(1, countEntries - 1);
    second = (first + dist(gen)) % countEntries;

    std::string firstPath, secondPath;
    std::size_t n = 0;
    for(auto& entry : std::filesystem::directory_iterator(pathToPictures)) {
        if (n == first)
            firstPath = entry.path().string();
        if (n == second)
            secondPath = entry.path().string();
        n++;
    }

    currentMenu = std::make_unique<MainMenu>(
        screen,
        pathToFont,
        firstPath,
        secondPath
    );
}