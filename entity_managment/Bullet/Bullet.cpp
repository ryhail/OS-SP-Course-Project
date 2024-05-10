//
// Created by hvorostok on 27.04.2024.
//

#include "Bullet.h"

Bullet::Bullet(bullet_t bulletInfo,
               const TextureHolder &bulletTexture) :
        Entity(sf::Vector2f(bulletInfo.coordinates.x, bulletInfo.coordinates.y)),
        sprite(bulletTexture.getResource(Textures::Bullet)) {
    if (bulletInfo.owner == 'b') {
        owner = BOSS;
    }
    if (bulletInfo.owner == 'p') {
        owner = PLAYER;
    }
    facing.x = 0;
    facing.y = 0;

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Bullet::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

Bullet::Bullet(sf::Vector2f _facing, sf::Vector2f _coordinates,
               Bullet::Owner _owner, const TextureHolder &textures):
        Entity(_coordinates){
    facing = _facing;
    owner = _owner;
    speed = BULLET_SPEED;

    sprite.setTexture(textures.getResource(Textures::Bullet));
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(coordinates);
}

void Bullet::updateCurrent(sf::Time dt, CommandQueue &queue) {
    coordinates += facing * dt.asSeconds();
    sprite.setPosition(coordinates);
}

bool Bullet::isForRemove(sf::RenderWindow &window) {
//    sf::Vector2f wind = window.getView().getSize();
//    return coordinates.x <= 0 || coordinates.x >= wind.x || coordinates.y <= 0 || coordinates.y >= wind.y;
    return false;
}

EntityType::Type Bullet::getCategory() const {
    return EntityType::BULLET;
}
