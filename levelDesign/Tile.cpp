#include "Tile.h"

Tile::Tile() : Sprite() {
    mType = Border;
}

void Tile::setType(Tile::Type type) {
    mType = type;
}

Tile::Type Tile::getType() {
    return mType;
}
