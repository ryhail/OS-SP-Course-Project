//
// Created by hvorostok on 16.05.2024.
//

#include "Pickup.h"
#include "../../player_managment/player/Player.h"
#include "../Data.h"
#include "../../states/utility.h"

namespace
{
    const std::vector<PickUpData> Table = fillPickUp();
}

bool Pickup::isForRemove() {
    return isUsed;
}

EntityType::Type Pickup::getCategory() const {
    return EntityType::PICKUP;
}

void Pickup::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

void Pickup::pickup(Player& player) {
    action(player);
}

bool Pickup::fitType(Pickup::Type _type) {
    return type == _type;
}

MapTile *Pickup::getCurrentMapTile() const {
    return currentMapTile;
}

void Pickup::use() {
    isUsed = true;
}

sf::FloatRect Pickup::getBoundingRect() const {
    return getWorldTransform()
    .transformRect(sprite.getGlobalBounds());
}

Pickup::Pickup(sf::Vector2f _coordinates, MapTile *_currentMapTile, int _type, TextureHolder &textures){

    coordinates = _coordinates;
    currentMapTile = _currentMapTile;

    if (_type == 0)
        type = Pickup::BulletRefill;
    else if(_type == 1)
        type = Pickup::HealthRefill;
    sprite.setTexture(textures.getResource(Table[type].texture));
    sprite.setColor(Table[type].color);
    sprite.setPosition(coordinates);
    centerOrigin(sprite);
    action = Table[type].pickupDoing;

    isUsed = false;
}

Pickup::Type Pickup::getPickUpType() {
    return type;
}
