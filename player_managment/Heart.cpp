//
// Created by hvorostok on 19.05.2024.
//

#include "Heart.h"
#include "../states/utility.h"

void Heart::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

Heart::Heart(TextureHolder &textures){
    sprite.setTexture(textures.getResource(Textures::Heart));
    broken = false;
    //centerOrigin(sprite);
}

bool Heart::isForRemove() {
    return broken;
}

EntityType::Type Heart::getCategory() const {
    return EntityType::HEART;
}

sf::FloatRect Heart::getBoundingRect() const {
    return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

void Heart::setPosition(sf::Vector2f position) {
    sprite.setPosition(position);
}

void Heart::broke() {
    broken = true;
}
