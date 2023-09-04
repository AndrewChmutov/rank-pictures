#include "rank_menu.hpp"

// Custom libraries
#include "menu_events.hpp"
#include "picture_record.hpp"
#include "transition_state.hpp"

// SDL libraries
#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <string>


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
    
    loadEntities(screen);

    startTransitionIn(TransitionState::FADE_IN);
}


void RankMenu::loadEntities(Screen& screen) {
    loadName(screen);
    loadPicture(screen);
    loadIndex(screen);
    loadWins(screen);
}


void RankMenu::loadName(Screen& screen) {
    loadLabel(screen, &nameTexture, pictures[index].name);
}


void RankMenu::loadPicture(Screen& screen) {
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
    float winrate = 1.0f * pictures[index].wins / pictures[index].total;
    winrateText = "Winrate: " + std::to_string(winrate) + "%";
    loadLabel(screen, &winrateTexture, winrateText);
}


void RankMenu::loadTotal(Screen& screen) {
    loadLabel(screen, &totalTexture, "Total: " + std::to_string(pictures[index].total));
}


void RankMenu::loadLabel(Screen& screen, SDL_Texture** tempTexture, const std::string& toShow) {
    freeTexture(tempTexture);

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
                startTransitionOut(TransitionState::FADE_OUT);
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
    if (index <= 0) {
        index = 0;
        return;
    }

    transitionState = TransitionState::LEFT_OUT;
    transitionProgress = 0.0f;
    delta = 0.05f;
    
    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
}


void RankMenu::toRight() {
    if (index >= pictures.size() - 1) {
        index = pictures.size() - 1;
        return;
    }

    transitionState = TransitionState::RIGHT_OUT;
    transitionProgress = 0.0f;
    delta = 0.05f;

    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
}


void RankMenu::startTransitionIn(TransitionState state) {
    transitionState = state;
    transitionProgress = 0.0f;
    delta = 0.05f;

    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
}


void RankMenu::startTransitionOut(TransitionState state) {
    transitionState = state;
    transitionProgress = 0.0f;
    delta = 0.03f;

    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
}


void RankMenu::startTransitionLeftIn() {
    transitionState = TransitionState::CHANGE;
    transitionProgress = 0.0f;
    delta = 0.05f;

    index = std::max(0, index - 1);

    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
}



void RankMenu::startTransitionRightIn() {
    transitionState = TransitionState::CHANGE;
    transitionProgress = 0.0f;
    delta = 0.05f;

    index = std::min(static_cast<int>(pictures.size() - 1), index + 1);

    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
}


void RankMenu::updateTransitionIn() {
    if (transitionState != TransitionState::FADE_IN && 
            transitionState != TransitionState::LEFT_IN &&
            transitionState != TransitionState::RIGHT_IN)
        return;

    if (transitionState == TransitionState::LEFT_IN) {
        updateTransitionLeftIn();
    }
    else if (transitionState == TransitionState::RIGHT_IN) {
        updateTransitionRightIn();
    }
    else
        updateTransitionDefaultIn();  

    transitionProgress += delta;

    if (transitionProgress >= 1.0f) {
        transitionState = TransitionState::NONE;
        transitionProgress = 1.0f;

        SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_NONE);
    }
}


void RankMenu::updateTransitionDefaultIn() {
    float acceleration = 2.0f * 1.25f * boxH, t = 1.0f - transitionProgress;

    pictureRect.y   += acceleration * t * t / 2;
    borders.y       += acceleration * t * t / 2;

    acceleration = 2.0f * (10 + nameRect.h);

    nameRect.y -= acceleration * t * t / 2;

    if (transitionProgress <= 0.33f) {
        acceleration = 2.0f * (winsRect.h);
        t = (0.33f - transitionProgress) / 0.33f;

        winsRect.y += acceleration * t * t / 2;
    }
}


void RankMenu::updateTransitionLeftIn() {
    float acceleration = 2.0f * 1.5f * boxW, t = 1 - transitionProgress;
    pictureRect.x   -= acceleration * t * t / 2;
    borders.x       -= acceleration * t * t / 2;
}

void RankMenu::updateTransitionRightIn() {
    float acceleration = 2.0f * 2.5f * boxW, t = 1 - transitionProgress;
    pictureRect.x   += acceleration * t * t / 2;
    borders.x       += acceleration * t * t / 2;
}


void RankMenu::updateTransitionOut() {
    if (transitionState != TransitionState::FADE_OUT && 
            transitionState != TransitionState::LEFT_OUT &&
            transitionState != TransitionState::RIGHT_OUT)
        return;

    if (transitionState == TransitionState::LEFT_OUT) {
        updateTransitionLeftOut();
    }
    else if (transitionState == TransitionState::RIGHT_OUT) {
        updateTransitionRightOut();
    }
    else
        updateTransitionDefaultOut();

    transitionProgress += delta;

    if (transitionProgress >= 1.0f) {
        transitionProgress = 1.0f;

        if (transitionState == TransitionState::LEFT_OUT)
            startTransitionLeftIn();
        else if (transitionState == TransitionState::RIGHT_OUT)
            startTransitionRightIn();
        else
            transitionState = TransitionState::END;
    }
}


void RankMenu::updateTransitionDefaultOut() {
    float acceleration = 2.0f * 1.5 * boxW, t = transitionProgress;

    pictureRect.x   -= acceleration * t * t / 2;
    borders.x       -= acceleration * t * t / 2;

    acceleration = 2.0f * (10 + nameRect.h);

    nameRect.y -= acceleration * t * t / 2;

    if (transitionProgress <= 0.33f) {
        acceleration = 2.0f * (winsRect.w);
        t = (transitionProgress) / 0.33f;

        winsRect.x += acceleration * t * t / 2;
    }
}


void RankMenu::updateTransitionLeftOut() {
    float acceleration = 2.0f * 3.0f * boxW, t = transitionProgress;
    pictureRect.x   += acceleration * t * t / 2;
    borders.x       += acceleration * t * t / 2;
}


void RankMenu::updateTransitionRightOut() {
    float acceleration = 2.0f * 2.0f * boxW, t = transitionProgress;
    pictureRect.x   -= acceleration * t * t / 2;
    borders.x       -= acceleration * t * t / 2;
}


void RankMenu::update(Screen &screen) {
    int windowX, windowY;
    screen.getSize(windowX, windowY);

    nameRect.y = 10;
    nameRect.w = nameFont * pictures[index].name.size();
    nameRect.h = static_cast<int>(2.4 * nameFont);
    nameRect.x = windowX / 2 - nameRect.w / 2;
    
    
    boxW = static_cast<int>(500.0f / 1280 * windowX); 
    boxH = static_cast<int>(500.0f / 720 * windowY);

    int imgX, imgY;
    SDL_QueryTexture(pictureTexture, NULL, NULL, &imgX, &imgY);
    
    float ratio = 1.0f * imgX / imgY;
    float ratioBox = 1.0f * boxW / boxH;


    if (ratio > ratioBox) {
        // width - full width of the box
        pictureRect.w = boxW;
        // by ratio formula
        pictureRect.h = 1.0f * boxW / ratio;

        pictureRect.x = static_cast<int>(1.0f * windowX / 2 - 1.0f * boxW * (displacement - 0.5f) - pictureRect.w / 2);
        pictureRect.y = windowY / 2 - pictureRect.h / 2;
    }
    else {
        // height - full height of the box
        pictureRect.h = boxH;
        // by ratio formula
        pictureRect.w = ratio * boxH;

        pictureRect.x = static_cast<int>(1.0f * windowX / 2 - 1.0f * boxW * (displacement - 0.5f) - pictureRect.w / 2);
        // pictureRect.x = static_cast<int>(1.0f * windowX / 2 - 1.0f * pictureRect.w * 3 / 4);
        pictureRect.y = windowY / 2 - pictureRect.h / 2;
    }

    borders = SDL_Rect{
        static_cast<int>(1.0f * windowX/2 - 1.0f * boxW * displacement),
        windowY / 2 - boxH / 2,
        boxW,
        boxH
    };


    indexRect = SDL_Rect {
        borders.x + borders.w + 20,
        borders.y,
        static_cast<int>(nameFont * indexText.size()),
        static_cast<int>(2.4f * nameFont)
    };

    
    winsRect = SDL_Rect {
        borders.x + borders.w + 20,
        indexRect.y + indexRect.h + 5,
        static_cast<int>(otherFont * winsText.size()),
        static_cast<int>(2.4f * otherFont)
    };


    auto previousState = transitionState;
    updateTransitionIn();
    updateTransitionOut();

    updateInfo(screen, previousState);
}


void RankMenu::updateInfo(Screen& screen, TransitionState previous) {
    if (transitionState != TransitionState::CHANGE)
        return;

    loadEntities(screen);
    
    if (previous == TransitionState::LEFT_OUT)
        transitionState = TransitionState::LEFT_IN;
    else if (previous == TransitionState::RIGHT_OUT)
        transitionState = TransitionState::RIGHT_IN;
    else
        transitionState = TransitionState::NONE;
}


void RankMenu::renderTransitionIn() {
    if (transitionState != TransitionState::FADE_IN && 
            transitionState != TransitionState::LEFT_IN &&
            transitionState != TransitionState::RIGHT_IN)
        return;

    SDL_SetTextureAlphaMod(pictureTexture, static_cast<int>(transitionProgress * 255));
    SDL_SetTextureAlphaMod(winsTexture, static_cast<int>(transitionProgress * 255));
}


void RankMenu::renderTransitionOut() {
    if (transitionState != TransitionState::FADE_OUT && 
            transitionState != TransitionState::LEFT_OUT &&
            transitionState != TransitionState::RIGHT_OUT)
        return;

    SDL_SetTextureAlphaMod(pictureTexture, 255 - static_cast<int>(transitionProgress * 255));
    SDL_SetTextureAlphaMod(winsTexture, 255 - static_cast<int>(transitionProgress * 255));
}


void RankMenu::render(Screen& screen) {
    screen.putBackground();

    renderTransitionIn();
    renderTransitionOut();

    screen.putTexturedRect(
        nameRect.x,
        nameRect.y,
        nameRect.w,
        nameRect.h,
        nameTexture
    );


    screen.putTexturedRect(
        indexRect.x,
        indexRect.y,
        indexRect.w,
        indexRect.h,
        indexTexture
    );


    screen.putTexturedRect(
        winsRect.x, 
        winsRect.y, 
        winsRect.w, 
        winsRect.h, 
        winsTexture
    );


    screen.putTexturedRect(
        pictureRect.x, 
        pictureRect.y, 
        pictureRect.w,
        pictureRect.h,
        pictureTexture
    );

    screen.putRect(
        borders.x,
        borders.y,
        borders.x + borders.w,
        borders.y + borders.h,
        128, 128, 128
    );


    screen.show();
}


RankMenu::~RankMenu() {
    freeEntities();
}