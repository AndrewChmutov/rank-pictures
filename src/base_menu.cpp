#include "base_menu.hpp"
#include "menu_events.hpp"


MenuEvent BaseMenu::handleEvents(Screen& screen) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return MenuEvent::EXIT;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return MenuEvent::EXIT;

            case SDL_WINDOWEVENT:
                int newWidth, newHeight;
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    newWidth = event.window.data1;
                    newHeight = event.window.data2;
                    
                    screen.resize(newWidth, newHeight);
                }
                else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
                    screen.maximize(newWidth, newHeight);
                    
                }
                break;

            default:
                return handleSpecificEvent(event, screen);
        }
    }

    return MenuEvent::NONE;
}


void BaseMenu::render(Screen& screen) {}

void BaseMenu::update(const Screen& screen) {}

void BaseMenu::resize(Screen& screen) {}