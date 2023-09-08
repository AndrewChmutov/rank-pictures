#pragma once

// Custom libraries
#include "base_menu.hpp"
#include "menu_events.hpp"
#include "screen.hpp"
#include "transition_state.hpp"
#include "picture_record.hpp"

// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL_render.h>

class RankMenu : public BaseMenu {
    // Pictures
    std::vector<PictureRecord>& pictures;
    int index;


    // Labels //

    // Font fot labels
    std::string pathToFont;
    int nameFont;
    int otherFont;

    // Name
    SDL_Rect nameRect;
    SDL_Texture* nameTexture;

    // # Index
    SDL_Rect indexRect;
    std::string indexText;
    SDL_Texture* indexTexture;

    // Wins
    SDL_Rect winsRect;
    std::string winsText;
    SDL_Texture* winsTexture;

    // Winrate
    SDL_Rect winrateRect;
    std::string winrateText;
    SDL_Texture* winrateTexture;

    // Total
    SDL_Rect totalRect;
    std::string totalText;
    SDL_Texture* totalTexture;

    // Picture
    SDL_Texture* pictureTexture;
    SDL_Rect pictureRect;

    // Technical details //

    // Borders
    SDL_Rect borders;
    int boxW, boxH;
    float displacement;

    // Transition
    TransitionState transitionState;
    float transitionProgress, delta;

    // Return value
    MenuEvent toReturn;

    // Loads all textures
    void loadEntities(Screen& screen);
    void loadName(Screen& screen);      // name texture
    void loadPicture(Screen& screen);   // picture texture
    void loadIndex(Screen& screen);     // index texture
    void loadWins(Screen& screen);      // wins texture
    void loadWinrate(Screen& screen);   // winrate texture
    void loadTotal(Screen& screen);     // total texture

    // Used for loading any label
    void loadLabel(Screen& screen, SDL_Texture** tempTexture, const std::string& toShow);

    // Free all textures
    void freeEntities();
    void freeTexture(SDL_Texture** texture);    // Free texture if needed

    // Transitions //

    // Starts transition
    // Changes transition state
    void startTransition(TransitionState state, float delta = 0.05f);  

    // Starts transition to left
    // - Sets the parameters for transition
    void startTransitionLeftRightIn();

    // Updates positions for transition in
    void updateTransitionIn();
    void updateTransitionDefaultIn();
    void updateTransitionLeftIn();
    void updateTransitionRightIn();

    // Updates positions for transition out
    void updateTransitionOut();
    void updateTransitionDefaultOut();
    void updateTransitionLeftOut();
    void updateTransitionRightOut();

    // Change information according to 
    void updateInfo(Screen& screen, TransitionState previous);

    // Renders transition in
    void renderTransitionIn();

    // Renders transition out
    void renderTransitionOut();

    // Starts transition to left picture
    // Makes sure index >= 0
    // Changes transition state
    void toLeft();

    // Starts transition to right picture
    // Makesu sure index < size
    // Changes transition state
    void toRight();

    // Handle menu-specific events:
    // - SPACE key 
    virtual MenuEvent handleSpecificEvent(const SDL_Event& event, Screen& screen) override;
public:
    RankMenu(Screen& screen, std::vector<PictureRecord>& pictures, std::string pathToFont);

    virtual MenuEvent handleEvents(Screen& screen) override;

    // Update menu
    virtual void update(Screen& screen) override;

    // Render menu
    virtual void render(Screen& screen) override;

    virtual ~RankMenu() override;
};