#include "application.hpp"
#include "menu_events.hpp"
#include <exception>
#include <memory>
#include "main_menu.hpp"


Application::Application(std::size_t w, std::size_t h, std::string pathToPictures, std::string pathToFont) :
    screen(w, h, "Picture ranking", pathToFont),
    pathToPictures(pathToPictures),
    currentMenu(std::make_unique<MainMenu>()){}

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
            currentMenu = std::make_unique<MainMenu>();

        case MenuEvent::TO_RATING_SCREEN:
        break;
    }
}