#include "application.hpp"
#include "menu_events.hpp"
#include "main_menu.hpp"
#include "picture_record.hpp"
#include <filesystem>
#include <random>


Application::Application(std::size_t w, std::size_t h, std::string pathToPictures, std::string pathToFont) :
    // Setup a screen
    screen(w, h, "Picture ranking"),

    // Setup main paths
    pathToPictures(pathToPictures),
    pathToFont(pathToFont),

    // Setup random generation
    gen(std::random_device()()){
    
    // Get all the current pictures in the directory
    pictures.clear();
    for(auto& entry : std::filesystem::directory_iterator(pathToPictures)) {
        if (entry.path().extension().string() == ".jpg" ||
                entry.path().extension().string() == ".png" ||
                entry.path().extension().string() == ".bmp") {
            pictures.push_back(PictureRecord{entry.path().filename().string(), 0, 0});
            pathPictures.push_back(entry.path().string());
        }

    }
    // Start from main menu
    switchToMain();
}


int Application::run() {
    isRunning = true;

    while (isRunning) {
        update();
        render();
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

        case MenuEvent::LEFT_CHOSEN:
            // TO-DO
            // std::cout << "left" << std::endl;
            switchToMain();
            break;
        case MenuEvent::RIGHT_CHOSEN:
            // TO-DO
            // std::cout << "right" << std::endl;
            switchToMain();
            break;
        // Main screen command
        case MenuEvent::TO_MAIN_SCREEN:
            switchToMain();
            break;

        // Rating screen command
        case MenuEvent::TO_RATING_SCREEN:
            // TO-DO
        
        // If no info is provided
        case MenuEvent::NONE:
            break;
    }

    // Update window and render
    currentMenu.get()->update(screen);
}


void Application::render() {
    currentMenu.get()->render(screen);
}


void Application::switchToMain() {
    // from 0 to size - 1
    dist = std::uniform_int_distribution<>(0, pictures.size() - 1);
    int left = dist(gen);

    // from first to first - 1 (cycle)
    dist = std::uniform_int_distribution<>(1, pictures.size());
    int right = (left + dist(gen)) % (pictures.size());

    currentMenu = std::make_unique<MainMenu>(
        screen,
        pathToFont,
        pathPictures[left],
        pathPictures[right]
    );
}