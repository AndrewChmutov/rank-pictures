#include "application.hpp"
#include "menu_events.hpp"
#include "main_menu.hpp"
#include "picture_record.hpp"
#include "rank_menu.hpp"
#include <filesystem>
#include <random>
#include <iostream>
#include <string>
#include <algorithm>


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
                entry.path().extension().string() == ".jpeg" ||
                entry.path().extension().string() == ".png" ||
                entry.path().extension().string() == ".bmp") {
            pictures.push_back(PictureRecord{entry.path().filename().string(), 0, 0});
            pathPictures.push_back(entry.path().string());
        }

    }
    lastLeft = -1;
    lastRight = -1;
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

        case MenuEvent::TO_MAIN_NEXT:
            switchToMain();
            break;

        // Main screen command
        case MenuEvent::TO_MAIN_SCREEN:
            lastLeft = -1;
            lastRight = -1;
            switchToMain();
            break;

        // Rating screen command
        case MenuEvent::TO_RATING_SCREEN:
            switchToRank();
            break;

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
    dist = std::uniform_int_distribution<>(1, pictures.size() - 1);
    int right = (left + dist(gen)) % (pictures.size());

    currentMenu = std::make_unique<MainMenu>(
        screen,
        pathToFont,
        pictures[left],
        pictures[right],
        pathPictures[left],
        pathPictures[right],
        (lastLeft == -1)? "" : std::to_string(pictures[lastLeft].wins),
        (lastRight== -1)? "" :std::to_string(pictures[lastRight].wins)
    );

    lastLeft = left;
    lastRight = right;
}


void Application::switchToRank() {
    std::sort(pictures.begin(), pictures.end(),
        [](const PictureRecord& first, const PictureRecord& second) {
            return first.wins > second.wins;
        }
    );

    currentMenu = std::make_unique<RankMenu>(
        screen,
        pictures[0],
        0,
        pictures.size(),
        pathToPictures,
        pathToFont
    );
}


void Application::debug() {
    for(auto& picture : pictures) {
        std::cout << "Name: " << picture.name << std::endl;
        std::cout << "Wins: " << picture.wins << std::endl;
        std::cout << "Total: " << picture.total << std::endl;
        std::cout << "Success rate: " << 1.0f * picture.wins / picture.total << std::endl << std::endl;
    }
}


Application::~Application() {
    // debug();
}