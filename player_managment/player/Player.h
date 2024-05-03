#ifndef COURSE_PLAYER_H
#define COURSE_PLAYER_H


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../entity_managment/Bullet/Bullet.h"
#include "../../entity_managment/Entity/EntityType.h"

#define PLAYER_INIT_SPEED 5

class Player : public Entity{
    sf::Vector2i    facing;
    sf::Sprite      playerSprite;
    int             hitPoints;
    int             speed;
    bool            active;
public:
    Player(TextureHolder *textures, Textures::ID playerType);
    void    takeDamage(int dmg);
    void    heal(int heal);
    void    updateFacing(int x, int y);
    void    setActive(bool value);
    void move(sf::Vector2i direction, sf::Time time);
    bool    isActive() const;
    void    draw(sf::RenderWindow* window);
    int     getSpeed() const;

    EntityType::Type getCategory() const override;
};


#endif //COURSE_PLAYER_H
