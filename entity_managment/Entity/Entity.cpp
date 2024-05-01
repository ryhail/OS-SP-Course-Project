#include "Entity.h"

Entity::Entity() {
    coordinates.x = 0;
    coordinates.y = 0;
}

void Entity::updateCurrent(coordinate _coordinate) {
    coordinates = _coordinate;
}
