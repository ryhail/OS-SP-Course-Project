#ifndef COURSE_PLAYER_H
#define COURSE_PLAYER_H


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../entity_managment/Bullet/Bullet.h"

class Player {
    sf::Vector2i coordinates;
    sf::Vector2i facing;
    sf::Sprite   playerSprite;
    int          hitPoints;
    bool         active;
public:
    Player(TextureHolder *textures, Textures::ID playerType);
    void takeDamage(int dmg);
    void heal(int heal);
    sf::Vector2i getCoordinates();
    void updateCoordinates(sf::Vector2i newCoords);
    void updateFacing(int x, int y);
    void setActive(bool value);
    bool isActive();
    void draw(sf::RenderWindow* window);
};


#endif //COURSE_PLAYER_H
