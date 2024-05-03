#include "Entity.h"

Entity::Entity() {
    coordinates.x = 0;
    coordinates.y = 0;
}

void Entity::updateCurrent(sf::Vector2i _coordinates) {
    coordinates.x = _coordinates.x;
    coordinates.y = _coordinates.y;
}

void Entity::updateCurrent(sf::Time dt) {
    //will be overriden
}

Entity::Entity(sf::Vector2i _coordinates) {
    coordinates = _coordinates;
}
