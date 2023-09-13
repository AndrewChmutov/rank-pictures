#pragma once

// Custom libraries
#include "base_menu.hpp"
#include "menu_events.hpp"
#include "picture_record.hpp"
#include "transition_state.hpp"

// C++ standard libraries
#include <random>

// SDL libraries
#include <SDL2/SDL_ttf.h>

class MainMenu : public BaseMenu {
    // Pictures //

    // Picture records
    std::vector<PictureRecord>& pictures;

    // Current pictures to show
    int currentLeft, currentRight;

    // Pictures
    SDL_Rect leftRect, rightRect;
    SDL_Texture* leftTexture, * rightTexture;

    // Picture boxes
    SDL_Rect leftBorders, rightBorders;
    int boxW, boxH;

    // Line separator
    int lineX1, lineY1, lineX2, lineY2;
    int lineMargin;


    // Labels //

    // Font
    TTF_Font* font;
    int fontSize;

    // Label
    std::string text;
    SDL_Rect labelRect;
    SDL_Texture* labelTexture;

    // Counters
    int leftWinner;
    std::string leftCounterText, rightCounterText;
    SDL_Rect leftCounterRect, rightCounterRect;
    SDL_Texture* leftCounterTexture,* rightCounterTexture,* counterWinnerTexture;


    // Technical details //

    // For app communication
    MenuEvent toReturn;

    // For choosing pictures
    std::mt19937 gen;
    std::uniform_int_distribution<int> dist;

    // Transition information
    TransitionState transitionState;
    float transitionProgress, delta;

    // Sizes of the window - for proper scaling
    int windowWidth, windowHeight;


    // Free texture
    void freeTexture(SDL_Texture** texture);

    // Choose two pictures:
    // - Set the choices to currentLeft and currentRight
    // - Set new textures
    // - start transition in
    void getRandomDouble(Screen& screen);

    // Update windowWidth and windowHeight fields
    void updateWindowSize(const Screen& screen);

    // Update borders for the pictures:
    // - Set XY position
    // - Set width and height
    void updateBorders();


    // Transitions //

    // Counter indicates amount of wins
    void setupCounters(Screen& screen);

    // Starts transition in:
    // - Changes transition state to FADE_IN or FADE_IN_FIRST
    // - Sets parameters for transition
    void startTransitionIn(float delta = 0.05f);

    // Starts transition out:
    // - Changes transition state to FADE_OUT
    // - Sets parameters for transition
    void startTransitionOut(float delta = 0.03f);

    // Blending mode to change opacity
    // - Pictures
    void blend();

    // Blending mode to default
    // - Pictures
    void resetBlend();

    // Updates transition if the state is FADE_IN
    void updateTransitionIn();
    // Updates transition if the state is FADE_OUT
    void updateTransitionOut();


    // Changes opacity of the pictures and moves them down
    void renderTransitionIn();
    // Changes opacity of the pictures and moves them sidewards
    void renderTransitionOut();

    // Render counters
    void renderCounters(Screen& screen);

    // Handle changes when left picture is chosen
    void leftWins();

    // Handle changes when right picture is chosen
    void rightWins();

    // Removes counters when transition ends.
    // Because counters are behind the pictures,
    // no need to render them if they are out
    // of the view.
    void removeCounters();


    // Obligatory implementation of a successor
    // Handles picture presses
    virtual MenuEvent handleSpecificEvent(const SDL_Event& event, Screen& screen) override; 
public:
    MainMenu(Screen& screen, std::vector<PictureRecord>& pictures, std::string& pathToFont);

    // If the toReturn value is set to exit,
    // the menu signals it to the application immediately
    // after update
    virtual MenuEvent handleEvents(Screen& screen) override;

    // Flag if the application has to update and render the menu
    virtual bool toUpdate() override;

    // Update changes of the menu
    virtual void update(Screen& screen) override;

    // Render menu
    virtual void render(Screen& scrern) override;


    virtual ~MainMenu() override;
};