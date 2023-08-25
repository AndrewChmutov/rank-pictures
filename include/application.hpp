#pragma once

#include "screen.hpp"
#include "base_menu.hpp"
#include "menu_events.hpp"
#include <cstddef>
#include <memory>
#include <random>

class Application {
    Screen screen;
    std::string pathToPictures;
    std::string pathToFont;
    std::unique_ptr<BaseMenu> currentMenu;
    bool isRunning;
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;

    void update();

    void switchToMain();
public:
    Application(std::size_t w, std::size_t h, std::string pathToPictures, std::string pathToFont);

    int run();

};