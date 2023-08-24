#include "main_menu.hpp"
#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <cstddef>


MainMenu::MainMenu(Screen& screen, std::string pathToFont, std::string pathToPic1, std::string pathToPic2) :
        rectLabel({0, 0, 0, 0}),
        rectLeft({0, 0, 0, 0}), 
        rectRight({0, 0, 0, 0}),
        fontSize(20),
        boxW(500),
        boxH(500) {
    font = TTF_OpenFont(pathToFont.c_str(), 200);

    if (font == NULL) {
        fprintf(stderr, "%s\n", "Could not initialize font!");
        exit(EXIT_FAILURE);
    }

    text = "Choose the better one!";

    SDL_Surface* temp = TTF_RenderText_Solid(
        font,
        text.c_str(),
        {0, 0, 0, 255}
    );
    textureLabel = screen.toTexture(temp);
    SDL_FreeSurface(temp);

    temp = IMG_Load(pathToPic1.c_str());
    textureLeft = screen.toTexture(temp);
    SDL_FreeSurface(temp);

    temp = IMG_Load(pathToPic2.c_str());
    textureRight = screen.toTexture(temp);
    SDL_FreeSurface(temp);
}


void MainMenu::handleSpecificEvent(const SDL_Event& event, Screen& screen) {
    
}

void MainMenu::update(const Screen& screen) {
    int windowX, windowY;
    screen.getSize(windowX, windowY);

    rectLabel.w = fontSize * text.size();
    rectLabel.h = 2.4 * fontSize;

    rectLabel.y = 10;
    rectLabel.x = windowX / 2 - rectLabel.w / 2;

    lineX1 = lineX2 = windowX / 2;

    lineY1 = rectLabel.y + rectLabel.h + 10;
    lineY2 = windowY - 10;


    int imgX, imgY;
    SDL_QueryTexture(textureLeft, NULL, NULL, &imgX, &imgY);

    float ratio = 1.0f * imgX / imgY;
    float ratioBox = 1.0f * boxW / boxH;

    if (ratio > ratioBox) {
        rectLeft.w = boxW;
        rectLeft.h = 1.0f * boxW / ratio;
    
        rectLeft.x = windowX / 2 - 60 - boxW;
        rectLeft.y = windowY / 2 - rectLeft.h / 2;
    }
    else {
        rectLeft.h = boxH;
        rectLeft.w = ratio * boxH;

        rectLeft.y = windowY / 2 - boxH / 2;
        rectLeft.x = windowX / 2 - 60 - boxW + (boxW / 2 - rectLeft.w / 2);
    }
}


void MainMenu::render(Screen& screen) {
    screen.putBackground();
    screen.putTexturedRect(
        rectLabel.x, 
        rectLabel.y, 
        rectLabel.w,
        rectLabel.h,
        textureLabel
    );

    screen.putTexturedRect(rectLeft.x, rectLeft.y, rectLeft.w, rectLeft.h, textureLeft);

    screen.putLine(lineX1, lineY1, lineX2, lineY2, 128, 128, 128);

    screen.show();
}


MainMenu::~MainMenu() {
    SDL_DestroyTexture(textureRight);
    SDL_DestroyTexture(textureLeft);
    SDL_DestroyTexture(textureLabel);

    TTF_CloseFont(font);
}