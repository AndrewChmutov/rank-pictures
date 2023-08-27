#pragma once

// Custom libraries
#include "base_menu.hpp"

class RankMenu : public BaseMenu {

public:
    virtual MenuEvent handleSpecificEvent(const SDL_Event& event, Screen& screen) override;
};