//
// Created by hvorostok on 16.05.2024.
//

#ifndef SFML_PICKUP_H
#define SFML_PICKUP_H


#include "../Entity/Entity.h"
#include "../Bullet/Bullet.h"
#include "../../player_managment/player/Player.h"

class Pickup : public Entity {
    typedef std::function<void(Player&)> PickupDoing;
public:
    enum Type
    {
        HealthRefill,
        BulletRefill,
        TypeCount
    };
public:
    Pickup(sf::Vector2f _coordinates, MapTile *_currentMapTile, Pickup::Type _type, TextureHolder &textures);
    void                pickup(Player& player);

    bool                isForRemove() override;
    EntityType::Type    getCategory() const override;
    sf::FloatRect       getBoundingRect() const override;

    MapTile*    getCurrentMapTile() const;
    void            use();
    Pickup::Type    getPickUpType()

private:

    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

    bool    fitType(Pickup::Type type);

private:
    bool            isUsed;
    Pickup::Type    type;
    PickupDoing     action;
    sf::Sprite      sprite;
    MapTile*        currentMapTile;
};


#endif //SFML_PICKUP_H
