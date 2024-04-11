#include "SFML/Graphics.hpp"
#ifndef SFML_CONNECTTOSERVER_H
#define SFML_CONNECTTOSERVER_H


class ConnectToServer {
    sf::Sprite backGround;
    sf::RectangleShape textBoard;
    sf::Text textField;
    std::string userInput;
    sf::Font font;
    bool status;
public:
    ConnectToServer();
    void update(sf::Event*);
    void draw(sf::RenderWindow* window);

};


#endif //SFML_CONNECTTOSERVER_H
