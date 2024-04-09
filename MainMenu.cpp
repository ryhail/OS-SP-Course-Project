#include "MainMenu.h"

MainMenu::MainMenu() {
    if(!menuFont.loadFromFile("resources/Fonts/SuperPixel.ttf")) {
        exit(1);
    }
    if(!textureBackground.loadFromFile("resources/Textures/menubg.png")) {
        exit(1);
    }

    status = false;

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

void MainMenu::update(sf::Event *event) {
    switch(event->type) {
        case(sf::Event::MouseMoved): {
            if(event->mouseMove.x > menuOptions[0].getPosition().x &&
            event->mouseMove.x < menuOptions[0].getPosition().x + menuOptions[0].getGlobalBounds().getSize().x) {
                if(event->mouseMove.y > menuOptions[0].getPosition().y &&
                   event->mouseMove.y < menuOptions[0].getPosition().y + menuOptions[0].getGlobalBounds().getSize().y) {
                        menuOptions[0].setFillColor(sf::Color::Red);
                } else {
                    menuOptions[0].setFillColor(sf::Color::White);
                }
            } else {
                menuOptions[0].setFillColor(sf::Color::White);
            }
            break;
        }
    }
}

bool MainMenu::isActive() {
    return this->status;
}
