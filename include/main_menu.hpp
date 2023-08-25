#pragma once

#include "base_menu.hpp"
#include <SDL2/SDL_ttf.h>

class MainMenu : public BaseMenu {
    SDL_Rect rectLabel, rectLeft, rectRight;
    SDL_Rect leftBorders, rightBorders;
    SDL_Texture* textureLabel, * textureLeft, * textureRight;
    TTF_Font* font;
    int lineX1, lineY1, lineX2, lineY2;
    int boxW, boxH;
    int fontSize;
    std::string text;

public:
    MainMenu(Screen& screen, std::string pathToFont, std::string pathToPic1, std::string pathToPic2);

    virtual void handleSpecificEvent(const SDL_Event& event, Screen& screen) override; 

    virtual void update(const Screen& screen) override;

    virtual void render(Screen& scrern) override;

    virtual ~MainMenu() override;
};