#pragma once

#include "screen.hpp"
#include "base_menu.hpp"
#include "menu_events.hpp"
#include <cstddef>
#include <memory>
#include <queue>

class Application {
    Screen screen;
    std::string pathToPictures;
    std::unique_ptr<BaseMenu> currentMenu;
    bool isRunning;

    void update();

public:
    Application(std::size_t w, std::size_t h, std::string pathToPictures, std::string pathToFont);

    int run();

};