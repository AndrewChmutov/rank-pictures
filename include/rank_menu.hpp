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
    // Font fot labels
    std::string pathToFont;

    // Name
    int nameFont;
    SDL_Rect nameRect;
    SDL_Texture* nameTexture;

    int otherFont;

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
    int boxW, boxH;

    SDL_Rect borders;
    float displacement;

    TransitionState transitionState;
    float transitionProgress, delta;

    std::vector<PictureRecord>& pictures;
    int index, size;

    MenuEvent eventTransition;
    MenuEvent toReturn;

    void loadEntities(Screen& screen);
    void loadName(Screen& screen);
    void loadPicture(Screen& screen);
    void loadIndex(Screen& screen);
    void loadWins(Screen& screen);
    void loadWinrate(Screen& screen);
    void loadTotal(Screen& screen);

    void loadLabel(Screen& screen, SDL_Texture** tempTexture, const std::string& toShow);

    void freeTexture(SDL_Texture** texture);
    void freeEntities();

    void startTransitionIn(TransitionState state);
    void startTransitionOut(TransitionState state);

    void updateTransitionIn();
    void updateTransitionDefaultIn();
    void updateTransitionLeftIn();
    void updateTransitionRightIn();

    void updateTransitionOut();
    void updateTransitionDefaultOut();
    void updateTransitionLeftOut();
    void updateTransitionRightOut();

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