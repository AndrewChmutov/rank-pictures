#include "application.hpp"
#include "menu_events.hpp"
#include <exception>
#include <memory>
#include "main_menu.hpp"
#include <filesystem>


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
    currentMenu = std::make_unique<MainMenu>(
        screen,
        pathToFont,
        "kek.jpg",
        "kek2.jpg"
    );
}