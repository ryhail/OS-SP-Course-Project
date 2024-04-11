//
// Created by egorz on 09.04.24.
//

#include "UserInterface.h"

void UserInterface::draw(sf::RenderWindow* window) {
    for(int i = 0; i < playerHitPoints; i++) {
        window->draw(this->heart);
        heart.setPosition(heart.getPosition()+sf::Vector2f(30,0));
    }
    heart.setPosition(20,20);
}

bool UserInterface::isActive() {
    return status;
}

void UserInterface::setStatus(bool status) {
    this->status = status;
}


UserInterface::UserInterface() {
    status = false;
    heart.setSize(sf::Vector2f(20,20));
    heart.setPosition(20,20);
    heart.setFillColor(sf::Color::Red);
}

void UserInterface::update(int playerHp) {
    this->playerHitPoints = playerHp;
}
