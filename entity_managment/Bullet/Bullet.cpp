//
// Created by hvorostok on 27.04.2024.
//

#include "Bullet.h"

Bullet::Bullet(bullet_t bulletInfo,
               const TextureHolder &bulletTexture) :
        Entity(), sprite(bulletTexture.getResource(Textures::BULLET)) {
    if (bulletInfo.owner == 'b') {
        owner = BOSS;
    }
    if (bulletInfo.owner == 'p') {
        owner = PLAYER;
    }
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Bullet::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}
