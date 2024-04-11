#include <SFML/Graphics.hpp>
#ifndef SFML_USERINTERFACE_H
#define SFML_USERINTERFACE_H


class UserInterface {
    sf::RectangleShape heart;
    int playerHitPoints;
    bool status;
public:
    UserInterface();
    bool isActive();
    void setStatus(bool status);
    void update(int playerHp);
    void draw(sf::RenderWindow* window);
};


#endif //SFML_USERINTERFACE_H
