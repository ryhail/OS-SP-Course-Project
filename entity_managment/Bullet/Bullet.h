#ifndef COURSE_BULLET_H
#define COURSE_BULLET_H

#include <SFML/Graphics/Sprite.hpp>
#include "../Entity/Entity.h"
#include "../../resources/ResourceHolder.h"
#include "../../levelDesign/MapTile.h"
#include <SFML/Graphics/RenderTarget.hpp>
#define BULLET_SPEED 180.0f

typedef ResourceHolder<Textures::ID, sf::Texture> TextureHolder;

class Bullet: public Entity {
public:
    Bullet(bullet_t bulletInfo, TextureHolder* bulletTexture);
    Bullet(sf::Vector2f _facing, sf::Vector2f _coordinates, EntityType::Type _owner, const TextureHolder& textures, MapTile* _currentMapTile);

    bool    isForRemove() override;
    void    use();

    EntityType::Type    getVictim() const;
    int                 getDamage() const;

    sf::FloatRect getBoundingRect() const override;
    void drawSprite(sf::RenderWindow* window);
    sf::Vector2f getCoordinates() override;
    sf::Vector2f getFacing();

private:
    void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateCurrent(sf::Time dt, CommandQueue &queue) override;

public:
    EntityType::Type getCategory() const override;

private:
    sf::Sprite          sprite;
    EntityType::Type    owner;
    sf::Vector2f        facing;
    float               speed;
    bool                isUsed;
    int                 level;
    int                 damage;
    MapTile*            currentMapTile;
};
#endif //COURSE_BULLET_H
