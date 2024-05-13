#ifndef COURSE_BULLET_H
#define COURSE_BULLET_H

#include <SFML/Graphics/Sprite.hpp>
#include "../Entity/Entity.h"
#include "../../resources/ResourceHolder.h"
#include <SFML/Graphics/RenderTarget.hpp>
#define BULLET_SPEED 120.0f

typedef ResourceHolder<Textures::ID, sf::Texture> TextureHolder;

class Bullet: public Entity{
public:
    enum Owner {
        PLAYER = 1 << 1,
        BOSS   = 1 << 2
    };
public:
    Bullet(bullet_t bulletInfo, const TextureHolder &bulletTexture);
    Bullet(sf::Vector2f _facing, sf::Vector2f _coordinates, Owner _owner, const TextureHolder& textures);

    bool isForRemove(sf::RenderWindow &window) override;

private:
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt, CommandQueue &queue) override;

public:
    EntityType::Type getCategory() const override;

private:
    sf::Sprite      sprite;
    Owner           owner;
    sf::Vector2f    facing;
    float           speed;
};
#endif //COURSE_BULLET_H
