#include "application.hpp"
#include "menu_events.hpp"
#include <exception>
#include <memory>
#include "main_menu.hpp"
#include <filesystem>
#include <iostream>
#include <random>


Application::Application(std::size_t w, std::size_t h, std::string pathToPictures, std::string pathToFont) :
    screen(w, h, "Picture ranking"),
    pathToPictures(pathToPictures),
    pathToFont(pathToFont),
    gen(std::random_device()()){

    switchToMain();
}

int Application::run() {
    isRunning = true;

    while (isRunning) {
        update();
        
        if (!isRunning)
            break;
    }

    return 0;
}


void Application::update() {
    MenuEvent event = currentMenu.get()->handleEvents(screen);

    switch (event) {
        case MenuEvent::EXIT:
            isRunning = false;
            break;

        case MenuEvent::TO_MAIN_SCREEN:
            switchToMain();

        case MenuEvent::TO_RATING_SCREEN:
        case MenuEvent::NONE:
            break;
    }

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

    dist = std::uniform_int_distribution<>(0, countEntries - 2);
    second = (first + dist(gen)) % (first - 1);

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