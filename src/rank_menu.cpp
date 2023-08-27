#include "rank_menu.hpp"
#include "menu_events.hpp"
#include <SDL_events.h>
#include <type_traits>


MenuEvent RankMenu::handleSpecificEvent(const SDL_Event &event, Screen &screen) {
    switch(event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                return MenuEvent::TO_MAIN_SCREEN;

    }

    return MenuEvent::NONE;
}