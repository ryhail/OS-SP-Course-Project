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
    bool _server_ip_present = false;
public:
    ConnectToServer();
    void update(sf::Event*);
    void draw(sf::RenderWindow* window);
    std::string getServerIp();
    bool getInputStatus();
};


#endif //SFML_CONNECTTOSERVER_H
