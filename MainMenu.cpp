#include "MainMenu.h"

MainMenu::MainMenu() {
    if(!menuFont.loadFromFile("resources/Fonts/SuperPixel.ttf")) {
        exit(1);
    }
    if(!textureBackground.loadFromFile("resources/Textures/menubg.png")) {
        exit(1);
    }
    menuBackground.setTexture(textureBackground);
    menuBackground.setPosition(0,0);
    menuOptions[0].setFillColor(sf::Color::White);
    menuOptions[0].setFont(menuFont);
    menuOptions[0].setString("PLAY");
    menuOptions[0].setCharacterSize(50);
    menuOptions[0].setPosition(550,300);
}

void MainMenu::draw(sf::RenderWindow* window) {
    window->draw(menuBackground);
    window->draw(menuOptions[0]);
}
