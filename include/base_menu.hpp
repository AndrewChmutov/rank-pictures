#pragma once

// Custom libraries
#include "menu_events.hpp"
#include "screen.hpp"

// Interface for menus
class BaseMenu {
    // For successors to implement
    // Called from handleEvents
    virtual MenuEvent handleSpecificEvent(const SDL_Event& event, Screen& screen) = 0;

public:
    // Handle default events
    // - Close
    // - Resize
    // - Maximize
    virtual MenuEvent handleEvents(Screen& screen);

    // Update menu
    virtual void update(Screen& screen);

    // Render menu
    virtual void render(Screen& screen);

    virtual ~BaseMenu() = default;

};