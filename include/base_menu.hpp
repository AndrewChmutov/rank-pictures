#pragma once

#include "menu_events.hpp"
#include "screen.hpp"


class BaseMenu {
public:
    virtual MenuEvent handleEvents(Screen& screen);

    virtual void handleSpecificEvent(const SDL_Event& event, Screen& screen) = 0;

    virtual void render();

    virtual void update();

    virtual void resize();

    virtual ~BaseMenu() = default;

};