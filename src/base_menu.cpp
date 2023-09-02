#include "base_menu.hpp"
#include "menu_events.hpp"

#include <iostream>
MenuEvent BaseMenu::handleEvents(Screen& screen) {
    // Event type to get queue of events
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            return MenuEvent::EXIT;

        // Choose action by type of event
        switch (event.type) {
            // If close button on the screen or ESC is pressed
            // then stop the app
            case SDL_QUIT:
                return MenuEvent::EXIT;
            

            // Handle all resizes
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

            // Handle specific event of the successor
            default:
                return handleSpecificEvent(event, screen);
        }
    }

    return MenuEvent::NONE;
}

void BaseMenu::update(Screen& screen) {}

void BaseMenu::render(Screen& screen) {}