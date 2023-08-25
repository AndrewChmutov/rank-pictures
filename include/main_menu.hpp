#pragma once

// Custom libraries
#include "base_menu.hpp"
#include "menu_events.hpp"

// SDL libraries
#include <SDL2/SDL_ttf.h>

class MainMenu : public BaseMenu {
    // Line coords
    int lineX1, lineY1, lineX2, lineY2;

    // Font size
    int fontSize;

    // Margin of pictures from the line
    int lineMargin;

    // Pictures
    SDL_Rect rectLeft, rectRight;
    SDL_Texture* textureLeft, * textureRight;

    // Label
    TTF_Font* font;
    std::string text;
    SDL_Rect rectLabel;
    SDL_Texture* textureLabel;

    // Picture boxes
    SDL_Rect leftBorders, rightBorders;
    int boxW, boxH;

public:
    MainMenu(Screen& screen, std::string pathToFont, std::string pathToPic1, std::string pathToPic2);

    // Obligatory implementation of a successor
    // Handles picture presses
    virtual MenuEvent handleSpecificEvent(const SDL_Event& event, Screen& screen) override; 

    // Update changes of the menu
    virtual void update(const Screen& screen) override;

    // Render menu
    virtual void render(Screen& scrern) override;

    virtual ~MainMenu() override;
};