#pragma once

#include "base_menu.hpp"

class MainMenu : public BaseMenu {
    SDL_Rect label, left, right;

public:
    virtual void handleSpecificEvent(const SDL_Event& event, Screen& screen) override; 
};