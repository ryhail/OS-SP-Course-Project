//
// Created by egorz on 09.04.24.
//

#include "TestServer.h"

TestServer::TestServer() {
    status = false;

    firstShape.setPosition(300, 300);
    firstShape.setSize(sf::Vector2f(50,50));
    firstShape.setFillColor(sf::Color::Red);

    secondShape.setPosition(300, 600);
    secondShape.setSize(sf::Vector2f(50,50));
    secondShape.setFillColor(sf::Color::Blue);

}

void TestServer::updatePositionFirst(sf::Vector2i pos) {
   this->firstShape.setPosition((sf::Vector2f)pos);
}

void TestServer::updatePositionSecond(sf::Vector2i pos) {
   this->firstShape.setPosition((sf::Vector2f)pos);
}

void TestServer::draw(sf::RenderWindow *window) {
    window->draw(firstShape);
    window->draw(secondShape);
}

bool TestServer::isActive() {
    return this->status;
}


