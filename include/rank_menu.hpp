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

class RankMenu : public BaseMenu {
    // Font fot labels
    std::string pathToFont;

    // Name
    int nameFont;
    SDL_Rect nameRect;
    SDL_Texture* nameTexture;

    int otherFont;

    // Wins
    SDL_Rect winsRect;
    std::string winsText;
    SDL_Texture* winsTexture;

    // Winrate
    SDL_Rect winrateRect;
    SDL_Texture* winrateTexture;

    // Total
    SDL_Rect totalRect;
    SDL_Texture* totalTexture;

    // Picture
    SDL_Texture* pictureTexture;
    SDL_Rect pictureRect;
    int boxW, boxH;

    SDL_Rect borders;
    float displacement;

    TransitionState transitionState;
    float transitionProgress, delta;

    std::vector<PictureRecord>& pictures;
    int index, size;

    MenuEvent eventTransition;
    MenuEvent toReturn;

    void loadName(Screen& screen);
    void loadPicture(Screen& screen);
    void loadWins(Screen& screen);

    void startTransitionIn();
    void startTransitionOut();

    void updateTransitionIn();
    void updateTransitionDefaultIn();

    void updateTransitionOut();
    void updateTransitionDefaultOut();

    void renderTransitionIn();
    void renderTransitionOut();

public:
    RankMenu(Screen& screen, std::vector<PictureRecord>& pictures, std::string pathToFont);

    virtual MenuEvent handleEvents(Screen& screen) override;

    virtual MenuEvent handleSpecificEvent(const SDL_Event& event, Screen& screen) override;

    virtual void update(Screen& screen) override;

    virtual void render(Screen& screen) override;

    virtual ~RankMenu() override;
};