#pragma once

// Custom libraries
#include "base_menu.hpp"
#include "transition_state.hpp"
#include "picture_record.hpp"

// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class RankMenu : public BaseMenu {
    SDL_Texture* picture;
    SDL_Texture* name,* wins,* winrate,* total;
    TTF_Font* font;
    
    TransitionState transitionState;
    float transitionProgress;
    float delta;

    PictureRecord& pictures;

    void startTransitionIn();
    void startTransitionOut();

    void updateTransitionIn();
    void updateTransitionOut();

public:
    RankMenu(PictureRecord& picture);

    virtual MenuEvent handleSpecificEvent(const SDL_Event& event, Screen& screen) override;

    virtual void update(const Screen& screen) override;
};