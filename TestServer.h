#include <SFML/Graphics.hpp>
#ifndef SFML_TESTSERVER_H
#define SFML_TESTSERVER_H


class TestServer {
    sf::RectangleShape firstShape;
    sf::RectangleShape secondShape;
    bool status;
public:
    TestServer();
    void updatePositionFirst(sf::Vector2i pos);
    void updatePositionSecond(sf::Vector2i pos);
    void draw(sf::RenderWindow* window);
    bool isActive();
};


#endif SFML_TESTSERVER_H
