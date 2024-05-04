#include "Entity.h"

Entity::Entity() {
    coordinates.x = 200;
    coordinates.y = 200;
}

void Entity::updateCurrent(const sf::Vector2f _coordinates) {
    coordinates.x = _coordinates.x;
    coordinates.y = _coordinates.y;
}

void Entity::updateCurrent(sf::Time dt) {
    //will be overriden
}

Entity::Entity(sf::Vector2f _coordinates) {
    coordinates = _coordinates;
}
