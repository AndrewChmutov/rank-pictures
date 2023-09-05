#include "main_menu.hpp"

// Custom libraries
#include "base_menu.hpp"
#include "menu_events.hpp"
#include "picture_record.hpp"
#include "transition_state.hpp"

// C++ standard libraries
#include <random>
#include <string>
#include <iostream>

// SDL libraries
#include <SDL2/SDL_image.h>


MainMenu::MainMenu(Screen& screen, std::vector<PictureRecord>& pictures, std::string& pathToFont) :
        font(nullptr),
        leftTexture(nullptr),
        rightTexture(nullptr),
        labelTexture(nullptr),
        leftCounterTexture(nullptr),
        rightCounterTexture(nullptr),
        counterWinnerTexture(nullptr),
        pictures(pictures),
        fontSize(20),
        boxW(500),
        boxH(500),
        lineMargin(60),
        leftWinner(-1),
        gen(std::random_device()()){
    // Setup font //
    // Open font
    font = TTF_OpenFont(pathToFont.c_str(), 50);

    // Check for errors
    if (font == NULL) {
        fprintf(stderr, "%s\n", "Could not initialize font!");
        exit(EXIT_FAILURE);
    }

    // The message for an upper label
    text = "Choose the better one!";

    //  Get texture of a font
    SDL_Surface* temp = TTF_RenderText_Blended(
        font,
        text.c_str(),
        {255, 255, 255, 255}
    );
    labelTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);

    // Get new 2 pictures
    getRandomDouble(screen);

    // Reset the transition state, that getRandomDouble()
    // establishes, to let the menu know, that it
    // is the first transition
    transitionState = TransitionState::FADE_IN_FIRST;
}


MenuEvent MainMenu::handleEvents(Screen& screen) {
    // If the end, return the toReturn immediately
    if (transitionState == TransitionState::END)
        return toReturn;

    // Invoke handleEvents from predecessor
    return BaseMenu::handleEvents(screen);
}


MenuEvent MainMenu::handleSpecificEvent(const SDL_Event& event, Screen& screen) {
    switch(event.type) {
        // If key is pressed
        case SDL_KEYDOWN:
            // Switch to rank menu if space is pressed
            if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && 
                    transitionState == TransitionState::NONE) {
                toReturn = MenuEvent::TO_RATING_SCREEN;
                startTransitionOut();
            }

            break;
        
        // Check the collision of a rect and a pointer if clicked
        case SDL_MOUSEBUTTONDOWN:
            // Get pointer position
            SDL_Point mousepoint;
            mousepoint.x = event.motion.x;
            mousepoint.y = event.motion.y;

            // Check if it is in a rect
            // Don't restart transition if it is running
            if (transitionState == TransitionState::NONE) {
                if (SDL_PointInRect(&mousepoint, &leftBorders)) {
                    toReturn = MenuEvent::NONE;
                    leftWins();
                    setupCounters(screen);
                    startTransitionOut();
                }
                else if (SDL_PointInRect(&mousepoint, &rightBorders)) {
                    toReturn = MenuEvent::NONE;
                    rightWins();
                    setupCounters(screen);
                    startTransitionOut();
                }
            }

            break;
    }

    return MenuEvent::NONE;
}


void MainMenu::getRandomDouble(Screen& screen) {
    // Randomly choose 2 picture indexes //

    dist = std::uniform_int_distribution<>(0, pictures.size() - 1);
    currentLeft = dist(gen);

    dist = std::uniform_int_distribution<>(1, pictures.size() - 1);
    currentRight = (currentLeft + dist(gen)) % pictures.size();

    // Free memory if needed //

    freeTexture(&leftTexture);
    freeTexture(&rightTexture);

    // Get textures for pictures //

    SDL_Surface* temp = IMG_Load(pictures[currentLeft].path.c_str());
    leftTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);

    temp = IMG_Load(pictures[currentRight].path.c_str());
    rightTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);


    // Start the transition to run the application
    startTransitionIn();
}


void MainMenu::updateWindowSize(const Screen& screen) {
    screen.getSize(windowWidth, windowHeight);
}


void MainMenu::updateBorders() {
    // 1280, 720 and 500.0f are literals,
    // basic scales of the program
    boxW = static_cast<int>(500.0f / 1280 * windowWidth); 
    boxH = static_cast<int>(500.0f / 720 * windowHeight);

    // Setup rectangles for printing //

    // Left
    leftBorders = SDL_Rect {
        windowWidth / 2 - boxW - 60, 
        windowHeight / 2 - boxH / 2,
        boxW,
        boxH,
    };

    // Right
    rightBorders = SDL_Rect {
        windowWidth / 2 + 60, 
        windowHeight / 2 - boxH / 2,
        boxW,
        boxH,
    };
}


void MainMenu::setupCounters(Screen& screen) {
    // Change text for counters
    leftCounterText = std::to_string(pictures[currentLeft].wins);
    rightCounterText = std::to_string(pictures[currentRight].wins);

    // Rectangles for rendering //

    // Left rectangle
    leftCounterRect.w = fontSize * leftCounterText.size();
    leftCounterRect.h = 2.4f * fontSize;
    leftCounterRect.x = leftBorders.x + leftBorders.w / 2 - leftCounterRect.w / 2;
    leftCounterRect.y = leftBorders.y + leftBorders.h / 2 - leftCounterRect.h / 2;

    // Right rectangle
    rightCounterRect.w = fontSize * rightCounterText.size();
    rightCounterRect.h = 2.4f * fontSize;
    rightCounterRect.x = rightBorders.x + rightBorders.w / 2 - rightCounterRect.w / 2;
    rightCounterRect.y = rightBorders.y + rightBorders.h / 2 - rightCounterRect.h / 2;


    // Free textures if needed //
    
    freeTexture(&leftCounterTexture);
    freeTexture(&rightCounterTexture);

    // Get textures of counters //

    // First texture
    SDL_Surface* temp = TTF_RenderText_Blended(
        font,
        leftCounterText.c_str(),
        {255, 255, 255, 255}
    );
    leftCounterTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);

    // Second texture
    temp = TTF_RenderText_Blended(
        font,
        rightCounterText.c_str(),
        {255, 255, 255, 255}
    );
    rightCounterTexture = screen.toTexture(temp);
    SDL_FreeSurface(temp);


    // if there is a winner to indicate
    if (leftWinner != -1) {
        // Free if needed
        freeTexture(&counterWinnerTexture);
        
        // Get texture
        temp = TTF_RenderText_Blended(
            font,
            leftWinner? leftCounterText.c_str() : rightCounterText.c_str(),
            {0, 240, 0, 255}
        );
        counterWinnerTexture = screen.toTexture(temp);
        SDL_FreeSurface(temp);
    }


}


void MainMenu::startTransitionIn(float delta) {
    // Set the state, parameters of the transition
    transitionState = TransitionState::FADE_IN;
    transitionProgress = 0.0f;
    this->delta = delta;

    // Set the mode BLEND for pictures for fading in
    blend();
}


void MainMenu::startTransitionOut(float delta) {
    // Set the state

    if (toReturn != MenuEvent::NONE)
        // If some return value is present,
        // the transition starts for switching
        transitionState = TransitionState::FADE_OUT_END;
    else
        // If nothing to return, that the transition is
        // for next pictures.
        transitionState = TransitionState::FADE_OUT;

    // Set parameters for transition
    transitionProgress = 0.0f;
    this->delta -= delta;

    // Set the mode BLEND for pictures for fading out
    blend();
}


void MainMenu::blend() {
    // Blend mode for two pictures
    SDL_SetTextureBlendMode(leftTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(rightTexture, SDL_BLENDMODE_BLEND);
}


void MainMenu::resetBlend() {
    // Normal mode for two pictures
    SDL_SetTextureBlendMode(leftTexture, SDL_BLENDMODE_NONE);
    SDL_SetTextureBlendMode(rightTexture, SDL_BLENDMODE_NONE);
}


void MainMenu::updateTransitionIn() {
    // Enter the method if conditions are met
    if (transitionState != TransitionState::FADE_IN &&
            transitionState != TransitionState::FADE_IN_FIRST)
        return;

    // Progress
    transitionProgress += delta;


    // Border rectangles and pictures //
    
    // Parameters for smooth transition
    float acceleration = 2.0f * 1.25f * boxH, t = 1.0f - transitionProgress;

    // Transition pictures
    leftRect.y  += acceleration * t * t / 2;
    rightRect.y += acceleration * t * t / 2;

    // Transition borders
    leftBorders.y   += acceleration * t * t / 2;
    rightBorders.y  += acceleration * t * t / 2;


    // if the first time, 
    if (transitionState == TransitionState::FADE_IN_FIRST) {
        // Pop up animation for label
        acceleration = 2.0f * (20 + labelRect.h);        
        labelRect.y -= acceleration * t * t / 2;
    }

    // End transition if the progress reaches end
    if (transitionProgress >= 1.0f) {
        transitionProgress = 1.0f;
        transitionState = TransitionState::NONE;

        // Unblen pictures
        resetBlend();
    }
}


void MainMenu::updateTransitionOut() {
    // Enter method if the conditions are met
    if (transitionState != TransitionState::FADE_OUT && 
            transitionState != TransitionState::FADE_OUT_END)
        return;

    // Progress
    transitionProgress += delta;

    // Border rectangles and transition //

    // Parameters for smooth transition
    float acceleration = 2.0f * 1.25f * boxW, t = 1.0f - transitionProgress;

    // Transition pictures
    leftRect.x -= 1.25f * boxW - acceleration * t * t / 2;
    rightRect.x += 1.25f * boxW - acceleration * t * t / 2;

    // Transition borders
    leftBorders.x -= 1.25f * boxW - acceleration * t * t / 2;
    rightBorders.x += 1.25f * boxW - acceleration * t * t / 2;


    // Hide the label if the last transition for this menu happens
    if (transitionState == TransitionState::FADE_OUT_END) {
        // Parameters for smooth transition
        acceleration = 2.0f * (20 + labelRect.h);
        t = transitionProgress;

        // Transition label
        labelRect.y -= acceleration * t * t / 2;
    }

    // End transition if progress reaches end
    if (transitionProgress >= 1.0f) {
        transitionProgress = 1.0f;
        leftWinner = -1;

        if (transitionState == TransitionState::FADE_OUT_END) {
            // If last transition for this menu,
            // Signal the menu to end
            transitionState = TransitionState::END;
        }
        else {
            // If not last,
            // Signal to change pictures
            transitionState = TransitionState::CHANGE;
        }
    }
}


void MainMenu::renderTransitionIn() {
    // Enter the method if the conditions are met
    if (transitionState != TransitionState::FADE_IN &&
            transitionState != TransitionState::FADE_IN_FIRST)
        return;
    
    // Change opacity of pictures (increasing)
    SDL_SetTextureAlphaMod(leftTexture, static_cast<int>(transitionProgress * 255));
    SDL_SetTextureAlphaMod(rightTexture, static_cast<int>(transitionProgress * 255));
}


void MainMenu::renderTransitionOut() {
    // Enter the method if the conditions are met
    if (transitionState != TransitionState::FADE_OUT &&
            transitionState != TransitionState::FADE_OUT_END)
        return;

    // Change opacity of pictures (decreasing)
    SDL_SetTextureAlphaMod(leftTexture, 255 - static_cast<int>(transitionProgress * 255));
    SDL_SetTextureAlphaMod(rightTexture, 255 - static_cast<int>(transitionProgress * 255));
}


void MainMenu::renderCounters(Screen& screen) {
    // Enter the method if the conditions are met
    if (transitionState != TransitionState::FADE_IN &&
            transitionState != TransitionState::FADE_OUT && 
            transitionState != TransitionState::CHANGE && 
            transitionState != TransitionState::END)
        return;

    // Ratio when to fade out winner counter (green one)
    float ratio = 0.7f;
    if (leftWinner != -1 && transitionProgress >= ratio) {
        SDL_SetTextureAlphaMod(counterWinnerTexture, static_cast<int>((1 - transitionProgress) / (1 - ratio) * 255));
    }

    // Put counters on the screen //

    // Left
    screen.putTexturedRect(
        leftCounterRect.x, 
        leftCounterRect.y, 
        leftCounterRect.w,
        leftCounterRect.h,
        leftCounterTexture
    );

    // Right
    screen.putTexturedRect(
        rightCounterRect.x, 
        rightCounterRect.y, 
        rightCounterRect.w,
        rightCounterRect.h,
        rightCounterTexture
    );

    // Winner (green one)
    if (!leftWinner) {
        screen.putTexturedRect(
            rightCounterRect.x, 
            rightCounterRect.y, 
            rightCounterRect.w,
            rightCounterRect.h,
            counterWinnerTexture
        );
    }
    else if (leftWinner == 1) {
        screen.putTexturedRect(
            leftCounterRect.x, 
            leftCounterRect.y, 
            leftCounterRect.w,
            leftCounterRect.h,
            counterWinnerTexture
        );
    }
}


void MainMenu::freeTexture(SDL_Texture** texture) {
    if (*texture)
        SDL_DestroyTexture(*texture);

    *texture = nullptr;
}


void MainMenu::removeCounters() {
    // If pictures hide counters, free counters
    if (transitionState == TransitionState::NONE) {
        // Free if needed
        freeTexture(&leftCounterTexture);
        freeTexture(&rightCounterTexture);
        freeTexture(&counterWinnerTexture);
    }
}


void MainMenu::update(Screen& screen) {
    // For proper positioning, get current size of the window
    // width and height
    updateWindowSize(screen);
    updateBorders();

    // Draw label //

    // Calculate the rect for the main label
    labelRect.w = fontSize * text.size();
    labelRect.h = 2.4 * fontSize;

    // Margin from upper edge
    labelRect.y = 10;

    // Place in the middle of the window int terms of X
    labelRect.x = windowWidth / 2 - labelRect.w / 2;


    // Draw central line //

    // Coords
    lineX1 = lineX2 = windowWidth / 2;
    lineY1 = labelRect.y + labelRect.h + 10;
    lineY2 = windowHeight - 10;
    // 10 and -10 are margins from edges

    // Draw left picture //

    // Get size of the picture
    int imgW, imgH;
    SDL_QueryTexture(leftTexture, NULL, NULL, &imgW, &imgH);

    // Fit the picture into the box //

    float ratio = 1.0f * imgW / imgH;
    float ratioBox = 1.0f * boxW / boxH;

    // If picture is wider
    if (ratio > ratioBox) {
        // Width - full width of the box
        leftRect.w = boxW;

        // Height calculated from the formula of ratio
        leftRect.h = 1.0f * boxW / ratio;
    
        // Position the picture
        leftRect.x = windowWidth / 2 - lineMargin - boxW;
        leftRect.y = windowHeight / 2 - leftRect.h / 2;
    }
    else {
        // Height - full height of the box
        leftRect.h = boxH;

        // Width calculated from the formula of ratio
        leftRect.w = ratio * boxH;

        // Position the picture
        leftRect.y = windowHeight / 2 - boxH / 2;
        leftRect.x = windowWidth / 2 - lineMargin - boxW / 2 - leftRect.w / 2;
    }

    // Draw right picture //

    // Get size of the picture
    SDL_QueryTexture(rightTexture, NULL, NULL, &imgW, &imgH);

    // Fit the picture into the box
    ratio = 1.0f * imgW / imgH;
    ratioBox = 1.0f * boxW / boxH;

    // If picture is wider
    if (ratio > ratioBox) {
        // Width - full width of the box
        rightRect.w = boxW;
        // By ratio formula
        rightRect.h = 1.0f * boxW / ratio;
    
        // Position the picture
        rightRect.x = windowWidth / 2 + 60;
        rightRect.y = windowHeight / 2 - rightRect.h / 2;
    }
    else {
        // Height - full height of the box
        rightRect.h = boxH;
        // By ratio formula
        rightRect.w = ratio * boxH;

        // Position the picture
        rightRect.y = windowHeight / 2 - boxH / 2;
        rightRect.x = windowWidth / 2 + 60 + boxW / 2 - rightRect.w / 2;
    }


    // Change pictures if needed
    // The state from CHANGE switches to FADE_IN
    if (transitionState == TransitionState::CHANGE) {
        getRandomDouble(screen);
    }

    // Update all transitions
    updateTransitionIn();
    updateTransitionOut();
}


void MainMenu::render(Screen& screen) {
    // Set the background
    screen.putBackground();

    // Print label
    screen.putTexturedRect(
        labelRect.x, 
        labelRect.y, 
        labelRect.w,
        labelRect.h,
        labelTexture
    );

    // render counters, if needed
    renderCounters(screen);
    // remove counters, if needed
    removeCounters();

    // Print pictures
    renderTransitionIn();
    renderTransitionOut();
    screen.putTexturedRect(leftRect.x, leftRect.y, leftRect.w, leftRect.h, leftTexture);
    screen.putTexturedRect(rightRect.x, rightRect.y, rightRect.w, rightRect.h, rightTexture);

    // Print borders //

    // Left
    screen.putRect(
        leftBorders.x, 
        leftBorders.y, 
        leftBorders.x + leftBorders.w,
        leftBorders.y + leftBorders.h,
        128, 128, 128
    );

    // Right
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


void MainMenu::leftWins() {
    pictures[currentLeft].wins++;
    pictures[currentLeft].total++;
    pictures[currentRight].total++;

    leftWinner = 1;
}


void MainMenu::rightWins() {
    pictures[currentRight].wins++;
    pictures[currentRight].total++;
    pictures[currentLeft].total++;

    leftWinner = 0;
}


MainMenu::~MainMenu() {
    freeTexture(&leftTexture);
    freeTexture(&rightTexture);
    freeTexture(&labelTexture);
    freeTexture(&leftCounterTexture);
    freeTexture(&rightCounterTexture);
    freeTexture(&counterWinnerTexture);

    if (font)
        TTF_CloseFont(font);
}