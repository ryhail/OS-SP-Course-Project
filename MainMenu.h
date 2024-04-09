#include <SFML/Graphics.hpp>

#ifndef MAINMENU_H
#define MAINMENU_H

#define MAX_MENU_OPTIONS 2

class MainMenu {
private:
    sf::Text menuOptions[MAX_MENU_OPTIONS];
    sf::Font menuFont;
    sf::Sprite menuBackground;
    sf::Texture textureBackground;
    bool status;
public:
    MainMenu();
    void draw(sf::RenderWindow* window);
    void update(sf::Event* event);
    bool isActive();
};


#endif //MAINMENU_H
