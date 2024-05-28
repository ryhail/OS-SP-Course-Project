//
// Created by hvorostok on 22.05.2024.
//

#ifndef SFML_BOSS_H
#define SFML_BOSS_H

#define BOSS_INIT_HITPOINTS 5

#include <SFML/Graphics/Sprite.hpp>
#include "Entity/Entity.h"
#include "Bullet/Bullet.h"
#include "../player_managment/Heart.h"

class Boss : public Entity{
public:
    Boss(sf::Vector2f _coordinates, int _hitPoints, TextureHolder& textures);

public:
    bool                isForRemove() override;
    EntityType::Type    getCategory() const override;
    sf::FloatRect       getBoundingRect() const override;
    void                takeDamage(int dmg);

    sf::Vector2f getCoordinates() override;
    void    move(sf::Vector2f coordinates);
    int getHitPoints() override;



private:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt, CommandQueue &queue) override;
    void animate(sf::Time dt);

private:
    sf::Vector2f    coordinates;
    sf::Sprite      sprite;
    sf::Time        animationDeltaTime;
    int             currentFrame;
    int             hitPoints;
};


#endif //SFML_BOSS_H
