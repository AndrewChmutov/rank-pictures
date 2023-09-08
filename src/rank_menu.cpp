#include "rank_menu.hpp"

// Custom libraries
#include "menu_events.hpp"
#include "picture_record.hpp"
#include "transition_state.hpp"

// C++ standard libraries
#include <iomanip>
#include <string>
#include <sstream>

// SDL libraries
#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>


RankMenu::RankMenu(Screen& screen, std::vector<PictureRecord>& pictures, std::string pathToFont) : 
        pictures(pictures), 
        index(0),
        transitionState(TransitionState::FADE_IN),
        displacement(1.0f),
        nameFont(20),
        otherFont(13),
        pathToFont(pathToFont),
        nameTexture(nullptr),
        indexTexture(nullptr),
        winsTexture(nullptr),
        winrateTexture(nullptr),
        totalTexture(nullptr),
        pictureTexture(nullptr) {
    
    // Load all the information needed
    loadEntities(screen);

    // Start the logic of the menu
    startTransition(TransitionState::FADE_IN);
}


void RankMenu::loadEntities(Screen& screen) {
    loadName(screen);
    loadPicture(screen);
    loadIndex(screen);
    loadWins(screen);
    loadWinrate(screen);
    loadTotal(screen);
}


void RankMenu::loadName(Screen& screen) {
    loadLabel(screen, &nameTexture, pictures[index].name);
}


void RankMenu::loadPicture(Screen& screen) {
    // if needed, free the pictures
    freeTexture(&pictureTexture);

    SDL_Surface* temp = IMG_Load(pictures[index].path.c_str());
    pictureTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);
}


void RankMenu::loadIndex(Screen& screen) {
    indexText = "#" + std::to_string(index + 1);
    loadLabel(screen, &indexTexture, indexText);
}


void RankMenu::loadWins(Screen& screen) {
    winsText = "Wins: " + std::to_string(pictures[index].wins);
    loadLabel(screen, &winsTexture, winsText);
}

void RankMenu::loadWinrate(Screen& screen) {
    // Calculating winrate
    float winrate;
    if (pictures[index].total == 0)
        winrate = 0;
    else
        winrate = 100.0f * pictures[index].wins / pictures[index].total;
    
    std::stringstream ss;
    ss << "Winrate: " << std::fixed << std::setprecision(2) << winrate << " %";
    winrateText = ss.str();
    loadLabel(screen, &winrateTexture, winrateText);
}


void RankMenu::loadTotal(Screen& screen) {
    totalText = "Total: " + std::to_string(pictures[index].total);
    loadLabel(screen, &totalTexture, totalText);
}


void RankMenu::loadLabel(Screen& screen, SDL_Texture** tempTexture, const std::string& toShow) {
    // free label if needed
    freeTexture(tempTexture);

    // Take the font
    TTF_Font* font = TTF_OpenFont(pathToFont.c_str(), 50);

    SDL_Surface* temp = TTF_RenderText_Blended(
        font, 
        toShow.c_str(),
        {255, 255, 255, 255}
    );

    *tempTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);
    TTF_CloseFont(font);
}


void RankMenu::freeTexture(SDL_Texture** texture) {
    if (*texture)
        SDL_DestroyTexture(*texture);

    *texture = nullptr;
}


void RankMenu::freeEntities() {
    freeTexture(&nameTexture);
    freeTexture(&pictureTexture);
    freeTexture(&indexTexture);
    freeTexture(&winsTexture);
    freeTexture(&winrateTexture);
    freeTexture(&totalTexture);
}


MenuEvent RankMenu::handleEvents(Screen& screen) {
    if (transitionState == TransitionState::END)
        return toReturn;

    return BaseMenu::handleEvents(screen);
}

MenuEvent RankMenu::handleSpecificEvent(const SDL_Event &event, Screen &screen) {
    switch(event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE &&
                    transitionState == TransitionState::NONE) {
                toReturn = MenuEvent::TO_MAIN_SCREEN;
                startTransition(TransitionState::FADE_OUT);
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT &&
                    transitionState == TransitionState::NONE) {
                toLeft();
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT &&
                    transitionState == TransitionState::NONE) {
                toRight();
            }

    }

    return MenuEvent::NONE;
}


void RankMenu::toLeft() {
    // No way to move left:
    // The best picture is displayed
    // according to ranking
    if (index <= 0) {
        index = 0;
        return;
    }

    // Safety measure
    index = std::max(0, index - 1);

    startTransition(TransitionState::LEFT_OUT);
}


void RankMenu::toRight() {
    // No way to move right:
    // The worst picture is displayed
    // according to ranking
    if (index >= pictures.size() - 1) {
        index = pictures.size() - 1;
        return;
    }

    // Safety measure
    index = std::min(static_cast<int>(pictures.size() - 1), index + 1);

    // Setup transition
    startTransition(TransitionState::RIGHT_OUT);
}


void RankMenu::startTransition(TransitionState state, float delta) {
    // Setup parameters
    transitionState = state;
    transitionProgress = 0.0f;
    this->delta = delta;

    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
}


void RankMenu::startTransitionLeftRightIn() {
    // Flag to change (update) the information
    startTransition(TransitionState::CHANGE);
}


void RankMenu::updateTransitionIn() {
    // Enter the methods if the confitions are met
    if (transitionState != TransitionState::FADE_IN && 
            transitionState != TransitionState::LEFT_IN &&
            transitionState != TransitionState::RIGHT_IN)
        return;

    // Specific transitions
    if (transitionState == TransitionState::LEFT_IN) {
        updateTransitionLeftIn();
    }
    else if (transitionState == TransitionState::RIGHT_IN) {
        updateTransitionRightIn();
    }
    else
        updateTransitionDefaultIn();  

    // Progress
    transitionProgress += delta;

    if (transitionProgress >= 1.0f) {
        // End of transition
        transitionState = TransitionState::NONE;
        transitionProgress = 1.0f;

        SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_NONE);
    }
}


void RankMenu::updateTransitionDefaultIn() {
    // Picture and borders
    float acceleration = 2.0f * 1.25f * boxH, t = 1.0f - transitionProgress;

    pictureRect.y   += acceleration * t * t / 2;
    borders.y       += acceleration * t * t / 2;

    // Name label
    acceleration = 2.0f * (20 + nameRect.h);

    nameRect.y -= acceleration * t * t / 2;

    // All other labels //
    acceleration = 2 * 0.2f * boxH;

    if (transitionProgress <= 0.33f) {
        // Wins
        t = 1 - transitionProgress / 0.33f;

        winsRect.y += acceleration * t * t / 2;
    }

    if (transitionProgress > 0.33f && transitionProgress <= 0.66f) {
        // Winrate
        t = 1 - (transitionProgress - 0.33f) / 0.33f;

        winrateRect.y += acceleration * t * t / 2;
    }

    if (transitionProgress > 0.66f) {
        // Total
        t = 1 - (transitionProgress - 0.66f) / 0.33f;

        totalRect.y += acceleration * t * t / 2;
    }
}


void RankMenu::updateTransitionLeftIn() {
    // Move only picture
    float acceleration = 2.0f * 1.5f * boxW, t = 1 - transitionProgress;

    pictureRect.x   -= acceleration * t * t / 2;
    borders.x       -= acceleration * t * t / 2;
}

void RankMenu::updateTransitionRightIn() {
    // Move only picture
    float acceleration = 2.0f * 2.5f * boxW, t = 1 - transitionProgress;

    pictureRect.x   += acceleration * t * t / 2;
    borders.x       += acceleration * t * t / 2;
}


void RankMenu::updateTransitionOut() {
    // Enter the method if the conditions are met
    if (transitionState != TransitionState::FADE_OUT && 
            transitionState != TransitionState::LEFT_OUT &&
            transitionState != TransitionState::RIGHT_OUT)
        return;

    // Specific transition
    if (transitionState == TransitionState::LEFT_OUT) {
        updateTransitionLeftOut();
    }
    else if (transitionState == TransitionState::RIGHT_OUT) {
        updateTransitionRightOut();
    }
    else
        updateTransitionDefaultOut();

    // Progress
    transitionProgress += delta;

    if (transitionProgress >= 1.0f) {
        // End of transition 
        transitionProgress = 1.0f;

        if (transitionState == TransitionState::LEFT_OUT)
            startTransitionLeftRightIn();
        else if (transitionState == TransitionState::RIGHT_OUT)
            startTransitionLeftRightIn();
        else
            transitionState = TransitionState::END;
    }
}


void RankMenu::updateTransitionDefaultOut() {
    // Picture
    float acceleration = 2.0f * 1.5 * boxW, t = transitionProgress;

    pictureRect.x   -= acceleration * t * t / 2;
    borders.x       -= acceleration * t * t / 2;

    // Name label
    acceleration = 2.0f * (10 + nameRect.h);

    nameRect.y -= acceleration * t * t / 2;

    // All other labels //

    acceleration = 2.0f * (0.3f * boxW);

    if (transitionProgress <= 0.33f) {
        // Wins
        t = (transitionProgress) / 0.33f;

        winsRect.x += acceleration * t * t / 2;
    }

    if (transitionProgress > 0.33f && transitionProgress < 0.66f) {
        // Winrate
        t = (transitionProgress - 0.33f) / 0.33f;

        winrateRect.x += acceleration * t * t / 2;
    }

    if (transitionProgress > 0.66f) {
        // Total
        t = (transitionProgress - 0.66f) / 0.33f;

        totalRect.x += acceleration * t * t / 2;
    }
}


void RankMenu::updateTransitionLeftOut() {
    // Move only picture
    float acceleration = 2.0f * 3.0f * boxW, t = transitionProgress;
    pictureRect.x   += acceleration * t * t / 2;
    borders.x       += acceleration * t * t / 2;
}


void RankMenu::updateTransitionRightOut() {
    // Move only picture
    float acceleration = 2.0f * 2.0f * boxW, t = transitionProgress;
    pictureRect.x   -= acceleration * t * t / 2;
    borders.x       -= acceleration * t * t / 2;
}


void RankMenu::update(Screen &screen) {
    // Update the information about the window
    int windowX, windowY;
    screen.getSize(windowX, windowY);
    boxW = static_cast<int>(500.0f / 1280 * windowX); 
    boxH = static_cast<int>(500.0f / 720 * windowY);

    // Name label
    nameRect.y = 10;
    nameRect.w = nameFont * pictures[index].name.size();
    nameRect.h = static_cast<int>(2.4 * nameFont);
    nameRect.x = windowX / 2 - nameRect.w / 2;
    
    // Picture //

    // Size of picture
    int imgWW, imgH;
    SDL_QueryTexture(pictureTexture, NULL, NULL, &imgWW, &imgH);
    
    float ratio = 1.0f * imgWW / imgH;
    float ratioBox = 1.0f * boxW / boxH;


    if (ratio > ratioBox) {
        // width - full width of the box
        pictureRect.w = boxW;
        // by ratio formula
        pictureRect.h = 1.0f * boxW / ratio;

        pictureRect.x = static_cast<int>(1.0f * windowX / 2 - 1.0f * boxW * (displacement - 0.5f) - 1.0f * pictureRect.w / 2);
        pictureRect.y = windowY / 2 - pictureRect.h / 2;
    }
    else {
        // height - full height of the box
        pictureRect.h = boxH;
        // by ratio formula
        pictureRect.w = ratio * boxH;

        pictureRect.x = static_cast<int>(1.0f * windowX / 2 - 1.0f * boxW * (displacement - 0.5f) - 1.0f * pictureRect.w / 2);
        // pictureRect.x = static_cast<int>(1.0f * windowX / 2 - 1.0f * pictureRect.w * 3 / 4);
        pictureRect.y = windowY / 2 - pictureRect.h / 2;
    }

    // Borders position
    borders = SDL_Rect{
        static_cast<int>(1.0f * windowX/2 - 1.0f * boxW * displacement),
        windowY / 2 - boxH / 2,
        boxW,
        boxH
    };

    // Index position
    indexRect = SDL_Rect {
        borders.x + borders.w + 20,
        borders.y,
        static_cast<int>(nameFont * indexText.size()),
        static_cast<int>(2.4f * nameFont)
    };

    // Wins position
    winsRect = SDL_Rect {
        borders.x + borders.w + 20,
        indexRect.y + indexRect.h + 5,
        static_cast<int>(otherFont * winsText.size()),
        static_cast<int>(2.4f * otherFont)
    };

    // Winrate position
    winrateRect = SDL_Rect {
        borders.x + borders.w + 20,
        winsRect.y + winsRect.h + 5,
        static_cast<int>(otherFont * winrateText.size()),
        static_cast<int>(2.4f * otherFont)
    };

    // Total position
    totalRect = SDL_Rect {
        borders.x + borders.w + 20,
        winrateRect.y + winrateRect.h + 5,
        static_cast<int>(otherFont * totalText.size()),
        static_cast<int>(2.4f * otherFont)
    };

    // In case of changes
    auto previousState = transitionState;

    // Transitions
    updateTransitionIn();
    updateTransitionOut();

    // If current state is CHANGE, then
    // update info
    updateInfo(screen, previousState);
}


void RankMenu::updateInfo(Screen& screen, TransitionState previous) {
    // If there is a necessity to change picture
    if (transitionState != TransitionState::CHANGE)
        return;

    // Reload entities
    loadEntities(screen);
    
    // Return back the state before CHANGE
    if (previous == TransitionState::LEFT_OUT)
        transitionState = TransitionState::LEFT_IN;
    else if (previous == TransitionState::RIGHT_OUT)
        transitionState = TransitionState::RIGHT_IN;
    else
        transitionState = TransitionState::NONE;
}


void RankMenu::renderTransitionIn() {
    // Enter the method if the conditions are met
    if (transitionState != TransitionState::FADE_IN && 
            transitionState != TransitionState::LEFT_IN &&
            transitionState != TransitionState::RIGHT_IN)
        return;
    
    // Change the opacity of the picture
    SDL_SetTextureAlphaMod(pictureTexture, static_cast<int>(transitionProgress * 255));

    if (transitionState == TransitionState::FADE_IN) {
        // If last transition in - order of fading

        if (transitionProgress <= 0.25f) {
            // 0.0 - 0.25
            SDL_SetTextureAlphaMod(indexTexture, static_cast<int>((transitionProgress / 0.25f) * 255));
        }

        if (transitionProgress <= 0.33f) {
            // 0.0 - 0.33
            SDL_SetTextureAlphaMod(winsTexture, static_cast<int>(transitionProgress / 0.33f * 255));
        }

        if (transitionProgress <= 0.33f) {
            // 0.0 - 0.33
            // Transparent
            SDL_SetTextureAlphaMod(winrateTexture, 0);
        }
        else if (transitionProgress > 0.33f && transitionProgress <= 0.66f) {
            // 0.34 - 0.66
            SDL_SetTextureAlphaMod(winrateTexture, static_cast<int>((transitionProgress - 0.33f) / 0.33f * 255));
        }

        if (transitionProgress <= 0.66f) {
            // 0.0 - 0.66
            // Transparent
            SDL_SetTextureAlphaMod(totalTexture, 0);
        }
        else {
            // 0.67 - 1.0
            SDL_SetTextureAlphaMod(totalTexture, static_cast<int>((transitionProgress - 0.66f) / 0.33f * 255));
        }

    }
    else {
        // If toLeft or toRight
        SDL_SetTextureAlphaMod(nameTexture, static_cast<int>(transitionProgress * 255));
        SDL_SetTextureAlphaMod(indexTexture, static_cast<int>(transitionProgress * 255));
        SDL_SetTextureAlphaMod(winsTexture, static_cast<int>(transitionProgress * 255));
        SDL_SetTextureAlphaMod(winrateTexture, static_cast<int>(transitionProgress * 255));
        SDL_SetTextureAlphaMod(totalTexture, static_cast<int>(transitionProgress * 255));
    }


}


void RankMenu::renderTransitionOut() {
    // Enter the method if the conditions are met
    if (transitionState != TransitionState::FADE_OUT && 
            transitionState != TransitionState::LEFT_OUT &&
            transitionState != TransitionState::RIGHT_OUT)
        return;

    // Change opacity of the picture
    SDL_SetTextureAlphaMod(pictureTexture, 255 - static_cast<int>(transitionProgress * 255));

    if (transitionState == TransitionState::FADE_OUT) {
        // If last fade out - order of fading

        if (transitionProgress <= 0.25f) {
            // 0.0 - 0.25
            SDL_SetTextureAlphaMod(indexTexture, 255 - static_cast<int>((transitionProgress / 0.25f) * 255));
        }
        else {
            // 0.26 - 1.0
            // Transaprent
            SDL_SetTextureAlphaMod(indexTexture, 0);
        }

        if (transitionProgress <= 0.33f) {
            // 0.0 - 0.33
            SDL_SetTextureAlphaMod(winsTexture, 255 - static_cast<int>(transitionProgress / 0.33f * 255));
        }
        else {
            // 0.34 - 1.0
            // Transparent
            SDL_SetTextureAlphaMod(winsTexture, 0);
        }

        if (transitionProgress <= 0.33f) {
            // 0.0 - 0.33
            // Full opacity
            SDL_SetTextureAlphaMod(winrateTexture, 255);
        }
        else if (transitionProgress > 0.33f && transitionProgress <= 0.66) {
            // 0.34 - 0.66
            SDL_SetTextureAlphaMod(winrateTexture, 255 - static_cast<int>((transitionProgress - 0.33f) / 0.33f * 255));
        }
        else {
            // 0.67 - 1.0
            // Transparent
            SDL_SetTextureAlphaMod(winrateTexture, 0);
        }

        if (transitionProgress <= 0.66f) {
            //  0.0 - 0.66
            // Full opacity
            SDL_SetTextureAlphaMod(totalTexture, 255);
        }
        else {
            // 0.67 - 1.0
            SDL_SetTextureAlphaMod(totalTexture, 255 - static_cast<int>((transitionProgress - 0.66f) / 0.33f * 255));
        }
    }
    else {
        // If toLeft or toRight
        SDL_SetTextureAlphaMod(nameTexture, 255 - static_cast<int>(transitionProgress * 255));
        SDL_SetTextureAlphaMod(indexTexture, 255 - static_cast<int>(transitionProgress * 255));
        SDL_SetTextureAlphaMod(winsTexture, 255 - static_cast<int>(transitionProgress * 255));
        SDL_SetTextureAlphaMod(winrateTexture, 255 - static_cast<int>(transitionProgress * 255));
        SDL_SetTextureAlphaMod(totalTexture, 255 - static_cast<int>(transitionProgress * 255));
    }

}


void RankMenu::render(Screen& screen) {
    // Put blank or textured background
    screen.putBackground();

    // Handle transitions
    renderTransitionIn();
    renderTransitionOut();

    // Render main entities //

    // Name label
    screen.putTexturedRect(
        nameRect.x,
        nameRect.y,
        nameRect.w,
        nameRect.h,
        nameTexture
    );

    // Index
    screen.putTexturedRect(
        indexRect.x,
        indexRect.y,
        indexRect.w,
        indexRect.h,
        indexTexture
    );

    // Wins
    screen.putTexturedRect(
        winsRect.x, 
        winsRect.y, 
        winsRect.w, 
        winsRect.h, 
        winsTexture
    );

    // Winrate
    screen.putTexturedRect(
        winrateRect.x, 
        winrateRect.y, 
        winrateRect.w, 
        winrateRect.h, 
        winrateTexture
    );

    // Total rounds
    screen.putTexturedRect(
        totalRect.x, 
        totalRect.y, 
        totalRect.w, 
        totalRect.h, 
        totalTexture
    );

    // Picture
    screen.putTexturedRect(
        pictureRect.x, 
        pictureRect.y, 
        pictureRect.w,
        pictureRect.h,
        pictureTexture
    );

    // Borders of picture
    screen.putRect(
        borders.x,
        borders.y,
        borders.x + borders.w,
        borders.y + borders.h,
        128, 128, 128
    );

    // Show changes on screen
    screen.show();
}


RankMenu::~RankMenu() {
    // Free all entities
    freeEntities();
}