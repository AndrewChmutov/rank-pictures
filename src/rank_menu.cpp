#include "rank_menu.hpp"

// Custom libraries
#include "menu_events.hpp"
#include "picture_record.hpp"

// SDL libraries
#include <SDL2/SDL_image.h>
#include <SDL_ttf.h>
#include <string>


RankMenu::RankMenu(Screen& screen, PictureRecord& picture, int index, int size, std::string path, std::string pathToFont, MenuEvent event) : 
        picture(picture), 
        transitionState(TransitionState::FADE_IN),
        index(index),
        size(size),
        displacement(1.0f),
        nameFont(20),
        otherFont(13),
        pathFont(pathToFont),
        eventTransition(event) {

    SDL_Surface* temp;

    temp = IMG_Load((path + "/" + picture.name).c_str());
    pictureTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);

    TTF_Font* font = TTF_OpenFont(pathToFont.c_str(), 50);

    temp = TTF_RenderText_Shaded(
        font, 
        picture.name.c_str(), 
        {0, 0, 0, 255}, 
        {255, 255, 255, 0}
    );

    nameTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);
    TTF_CloseFont(font);


    font = TTF_OpenFont(pathToFont.c_str(), 50);
    winsText = "Wins: " + std::to_string(picture.wins);
    temp = TTF_RenderText_Shaded(
        font, 
        winsText.c_str(),
        {0, 0, 0, 255}, 
        {255, 255, 255, 0}
    );

    winsTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);
    TTF_CloseFont(font);

    startTransitionIn();
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
                startTransitionOut();
            }

    }

    return MenuEvent::NONE;
}


void RankMenu::startTransitionIn() {
    transitionState = TransitionState::FADE_IN;
    transitionProgress = 0.0f;
    delta = 0.005f;

    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(winsTexture, SDL_BLENDMODE_BLEND);
}


void RankMenu::startTransitionOut() {
    transitionState = TransitionState::FADE_OUT;
    transitionProgress = 0.0f;
    delta = 0.002f;

    SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(winsTexture, SDL_BLENDMODE_BLEND);
}


void RankMenu::updateTransitionIn() {
    if (transitionState != TransitionState::FADE_IN)
        return;

    transitionProgress += delta;

    if (false) {}
    else
        updateTransitionDefaultIn();  

    if (transitionProgress >= 1.0f) {
        transitionProgress = 1.0f;
        transitionState = TransitionState::NONE;

        SDL_SetTextureBlendMode(pictureTexture, SDL_BLENDMODE_NONE);
        SDL_SetTextureBlendMode(winsTexture, SDL_BLENDMODE_NONE);
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


void RankMenu::updateTransitionOut() {
    if (transitionState != TransitionState::FADE_OUT)
        return;

    transitionProgress += delta;

    if (false) {}
    else
        updateTransitionDefaultOut();

    if (transitionProgress >= 1.0f) {
        transitionProgress = 1.0f;
        transitionState = TransitionState::END;
    }
}


void RankMenu::updateTransitionDefaultOut() {
    float acceleration = 2.0f * 1.25f * boxW, t = transitionProgress;

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


void RankMenu::update(const Screen &screen) {
    int windowX, windowY;
    screen.getSize(windowX, windowY);

    nameRect.y = 10;
    nameRect.w = nameFont * picture.name.size();
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

    
    winsRect = SDL_Rect {
        borders.x + borders.w + 20,
        borders.y + static_cast<int>(0.1f * borders.h),
        static_cast<int>(otherFont * winsText.size()),
        static_cast<int>(2.4f * otherFont)
    };


    updateTransitionIn();
    updateTransitionOut();
}


void RankMenu::renderTransitionIn() {
    if (transitionState != TransitionState::FADE_IN)
        return;

    SDL_SetTextureAlphaMod(pictureTexture, static_cast<int>(transitionProgress * 255));
    SDL_SetTextureAlphaMod(winsTexture, static_cast<int>(transitionProgress * 255));
}


void RankMenu::renderTransitionOut() {
    if (transitionState != TransitionState::FADE_OUT)
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
        winsRect.x, 
        winsRect.y, 
        winsRect.w, 
        winsRect.h, 
        winsTexture
    );


    screen.putRect(
        borders.x,
        borders.y,
        borders.x + borders.w,
        borders.y + borders.h,
        128, 128, 128
    );

    screen.putTexturedRect(
        pictureRect.x, 
        pictureRect.y, 
        pictureRect.w,
        pictureRect.h,
        pictureTexture
    );

    screen.show();
}


RankMenu::~RankMenu() {
    SDL_DestroyTexture(winsTexture);
    SDL_DestroyTexture(nameTexture);
    SDL_DestroyTexture(pictureTexture);
}