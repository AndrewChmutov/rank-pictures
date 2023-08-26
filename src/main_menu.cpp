#include "main_menu.hpp"

// Custom libraries
#include "base_menu.hpp"
#include "transition_state.hpp"

// SDL libraries
#include <SDL2/SDL_image.h>

MainMenu::MainMenu(Screen& screen, std::string pathToFont, std::string pathToPic1, std::string pathToPic2) :
        rectLabel({0, 0, 0, 0}),
        rectLeft({0, 0, 0, 0}), 
        rectRight({0, 0, 0, 0}),
        fontSize(20),
        boxW(500),
        boxH(500),
        lineMargin(60),
        transitionState(TransitionState::FADE_IN),
        transitionProgress(0.0f) {
    // Setup font //
    // Open font
    font = TTF_OpenFont(pathToFont.c_str(), 200);

    // Check for errors
    if (font == NULL) {
        fprintf(stderr, "%s\n", "Could not initialize font!");
        exit(EXIT_FAILURE);
    }

    // The message for an upper label
    text = "Choose the better one!";

    //  Get texture of a font
    SDL_Surface* temp = TTF_RenderText_Solid(
        font,
        text.c_str(),
        {0, 0, 0, 255}
    );
    textureLabel = screen.toTexture(temp);
    SDL_FreeSurface(temp);

    // Get texture of the first image
    temp = IMG_Load(pathToPic1.c_str());
    textureLeft = screen.toTexture(temp);
    SDL_FreeSurface(temp);

    // Get texture of the second image
    temp = IMG_Load(pathToPic2.c_str());
    textureRight = screen.toTexture(temp);
    SDL_FreeSurface(temp);
}


void MainMenu::startTransitionOut() {
    transitionState = TransitionState::FADE_OUT;
    transitionProgress = 0.0f;
    delta = 0.002f;

    SDL_SetTextureBlendMode(textureLeft, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(textureRight, SDL_BLENDMODE_BLEND);
}


void MainMenu::updateTransitionOut() {
    if (transitionState != TransitionState::FADE_OUT)
        return;

    float acceleration = 2.0f * 1.25f * boxW, t = 1.0f - transitionProgress;


    rectLeft.x -= 1.25f * boxW - acceleration * t * t / 2;
    leftBorders.x -= 1.25f * boxW - acceleration * t * t / 2;

    rectRight.x += 1.25f * boxW - acceleration * t * t / 2;
    rightBorders.x += 1.25f * boxW - acceleration * t * t / 2;

    transitionProgress += delta;

    if (transitionProgress >= 1.0f) {
        transitionProgress = 1.0f;
        transitionState = TransitionState::END;
    }
}


void MainMenu::renderTransitionOut() {
    SDL_SetTextureAlphaMod(textureLeft, 255 - static_cast<int>(transitionProgress * 255));
    SDL_SetTextureAlphaMod(textureRight, 255 - static_cast<int>(transitionProgress * 255));
}


MenuEvent MainMenu::handleEvents(Screen& screen) {
    if (transitionState == TransitionState::END)
        return toReturn;

    return BaseMenu::handleEvents(screen);
}



MenuEvent MainMenu::handleSpecificEvent(const SDL_Event& event, Screen& screen) {
    switch(event.type) {
        // Check the collision of a rect and a pointer if clicked
        case SDL_MOUSEBUTTONDOWN:
            // Get pointer position
            SDL_Point mousepoint;
            mousepoint.x = event.motion.x;
            mousepoint.y = event.motion.y;

            // Check if it is in a rect
            // Don't restart transition if it is running
            if (transitionState != TransitionState::FADE_OUT) {
                if (SDL_PointInRect(&mousepoint, &leftBorders)) {
                    toReturn = MenuEvent::LEFT_CHOSEN;
                    startTransitionOut();
                }
                else if (SDL_PointInRect(&mousepoint, &rightBorders)) {
                    toReturn = MenuEvent::RIGHT_CHOSEN;
                    startTransitionOut();
                }
            }

            break;
    }

    return MenuEvent::NONE;
}

void MainMenu::update(const Screen& screen) {
    // For proper positioning, get current size of the window
    // width and height
    int windowX, windowY;
    screen.getSize(windowX, windowY);

    // Calculate the sizes for that borders
    boxW = static_cast<int>(500.0f / 1280 * windowX); 
    boxH = static_cast<int>(500.0f / 720 * windowY);

    // Draw label

    // Calculate the rect for the main label
    rectLabel.w = fontSize * text.size();
    rectLabel.h = 2.4 * fontSize;

    // Margin from upper edge
    rectLabel.y = 10;

    // Place in the middle of the window int terms of X
    rectLabel.x = windowX / 2 - rectLabel.w / 2;


    // Draw central line

    // Coords
    lineX1 = lineX2 = windowX / 2;
    lineY1 = rectLabel.y + rectLabel.h + 10;
    lineY2 = windowY - 10;
    // 10 and -10 are margins from edges

    // Draw pictures

    // Get size
    int imgX, imgY;
    SDL_QueryTexture(textureLeft, NULL, NULL, &imgX, &imgY);

    // Fit into the boxes
    float ratio = 1.0f * imgX / imgY;
    float ratioBox = 1.0f * boxW / boxH;

    // if picture is wider
    if (ratio > ratioBox) {
        // width - full width of the box
        rectLeft.w = boxW;
        // height calculated from the formula of ratio
        rectLeft.h = 1.0f * boxW / ratio;
    
        rectLeft.x = windowX / 2 - lineMargin - boxW;
        rectLeft.y = windowY / 2 - rectLeft.h / 2;
    }
    else {
        // height - full height of the box
        rectLeft.h = boxH;
        // width calculated from the formula of ratio
        rectLeft.w = ratio * boxH;

        rectLeft.y = windowY / 2 - boxH / 2;
        rectLeft.x = windowX / 2 - lineMargin - boxW / 2 - rectLeft.w / 2;
    }

    // Draw another picture

    // Get size
    SDL_QueryTexture(textureRight, NULL, NULL, &imgX, &imgY);

    // Fit into the boxes
    ratio = 1.0f * imgX / imgY;
    ratioBox = 1.0f * boxW / boxH;

    // If picture is wider
    if (ratio > ratioBox) {
        // width - full width of the box
        rectRight.w = boxW;
        // by ratio formula
        rectRight.h = 1.0f * boxW / ratio;
    
        rectRight.x = windowX / 2 + 60;
        rectRight.y = windowY / 2 - rectRight.h / 2;
    }
    else {
        // height - full height of the box
        rectRight.h = boxH;
        // by ratio formula
        rectRight.w = ratio * boxH;

        rectRight.y = windowY / 2 - boxH / 2;
        rectRight.x = windowX / 2 + 60 + boxW / 2 - rectRight.w / 2;
    }

    // Draw borders

    leftBorders = SDL_Rect {
        windowX/2 - boxW - 60, 
        windowY / 2 - boxH / 2,
        boxW,
        boxH,
    };

    rightBorders = SDL_Rect {
        windowX/2 + 60, 
        windowY / 2 - boxH / 2,
        boxW,
        boxH,
    };

    updateTransitionOut();
}


void MainMenu::render(Screen& screen) {
    // Set white screen
    screen.putBackground();

    // Print label
    screen.putTexturedRect(
        rectLabel.x, 
        rectLabel.y, 
        rectLabel.w,
        rectLabel.h,
        textureLabel
    );

    // Print pictures
    if (transitionState == TransitionState::FADE_OUT)
        renderTransitionOut();
    screen.putTexturedRect(rectLeft.x, rectLeft.y, rectLeft.w, rectLeft.h, textureLeft);
    screen.putTexturedRect(rectRight.x, rectRight.y, rectRight.w, rectRight.h, textureRight);

    // Print borders
    screen.putRect(
        leftBorders.x, 
        leftBorders.y, 
        leftBorders.x + leftBorders.w,
        leftBorders.y + leftBorders.h,
        128, 128, 128
    );

    screen.putRect(
        rightBorders.x, 
        rightBorders.y, 
        rightBorders.x + rightBorders.w,
        rightBorders.y + rightBorders.h,
        128, 128, 128
    );

    // Print central line
    screen.putLine(lineX1, lineY1, lineX2, lineY2, 128, 128, 128);

    // Show changed frame
    screen.show();
}


MainMenu::~MainMenu() {
    SDL_DestroyTexture(textureRight);
    SDL_DestroyTexture(textureLeft);
    SDL_DestroyTexture(textureLabel);

    TTF_CloseFont(font);
}