#pragma once

// Custom libraries
#include "base_menu.hpp"
#include "menu_events.hpp"
#include "picture_record.hpp"
#include "transition_state.hpp"

// SDL libraries
#include <SDL2/SDL_ttf.h>

class MainMenu : public BaseMenu {
    int leftWinner;

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

    // Counters
    std::string counterLeft, counterRight;
    SDL_Rect counterRectLeft, counterRectRight;
    SDL_Texture* counterTextureLeft,* counterTextureRight,* counterWinner;

    // Picture boxes
    SDL_Rect leftBorders, rightBorders;
    int boxW, boxH;

    // Pictures
    PictureRecord& recordLeft,& recordRight;

    // For app communication
    MenuEvent toReturn;

    // Transition
    TransitionState transitionState;
    float transitionProgress, delta;

    // Starts transition to another menu
    void startTransitionIn();
    void startTransitionOut();

    // Updates transition if the state is FADE_IN
    void updateTransitionIn();
    // Updates transition if the state is FADE_OUT
    void updateTransitionOut();

    // Counter indicates amount of wins
    void setupCounters(std::string counterLeft, std::string counterRight, Screen& screen);
    void updateCounters();

    // Changes opacity of the pictures and moves them down
    void renderTransitionIn();
    // Changes opacity of the pictures and moves them sidewards
    void renderTransitionOut();

    void renderCounters(Screen& screen);

    // Blending mode to change opacity
    void blend();

    // Blending mode to default
    void resetBlend();

    // Handle when left picture is chosen
    void leftWins();

    // Handle when right picture is chosen
    void rightWins();


    void removeCounters();
public:
    MainMenu(Screen& screen, std::string& pathToFont, PictureRecord& recordLeft, PictureRecord& recordRight, 
                std::string& pathToPicLeft, std::string& pathToPicRight,
                std::string lastLeft, std::string lastRight);
    virtual MenuEvent handleEvents(Screen& screen) override;

    // Obligatory implementation of a successor
    // Handles picture presses
    virtual MenuEvent handleSpecificEvent(const SDL_Event& event, Screen& screen) override; 

    // Update changes of the menu
    virtual void update(const Screen& screen) override;

    // Render menu
    virtual void render(Screen& scrern) override;


    virtual ~MainMenu() override;
};