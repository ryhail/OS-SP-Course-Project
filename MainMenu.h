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
public:
    MainMenu();
    void draw(sf::RenderWindow* window);
};


#endif //MAINMENU_H
