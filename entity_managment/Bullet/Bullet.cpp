//
// Created by hvorostok on 27.04.2024.
//

#include "Bullet.h"

Bullet::Bullet(bullet_t bulletInfo,
               TextureHolder* bulletTexture) :
        Entity(sf::Vector2f(bulletInfo.coordinates.x, bulletInfo.coordinates.y)),
        sprite(bulletTexture->getResource(Textures::Bullet)) {
    if (bulletInfo.owner == 'b') {
        owner = EntityType::BOSS;
    }
    if (bulletInfo.owner == 'p') {
        owner = EntityType::PLAYER;
    }
    isUsed = false;
    facing.x = 0;
    facing.y = 0;

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(sf::Vector2f(bulletInfo.coordinates.x, bulletInfo.coordinates.y));
}

void Bullet::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

Bullet::Bullet(sf::Vector2f _facing, sf::Vector2f _coordinates,
               EntityType::Type _owner, const TextureHolder &textures, MapTile* _currentMapTile):
        Entity(_coordinates){
    facing = _facing;
    owner = _owner;
    currentMapTile = _currentMapTile;
    level = 1;
    damage = 1;
    speed = BULLET_SPEED;
    isUsed = false;

    sprite.setTexture(textures.getResource(Textures::Bullet));
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(coordinates - bounds.getSize() * 0.5f);
}

void Bullet::updateCurrent(sf::Time dt, CommandQueue &queue) {
    coordinates += facing * speed * dt.asSeconds();
    sprite.setPosition(coordinates);
}

bool Bullet::isForRemove() {
    return true;
}

EntityType::Type Bullet::getCategory() const {
    return EntityType::BULLET;
}

sf::FloatRect Bullet::getBoundingRect() const {
    return getWorldTransform()
            .transformRect(sprite.getGlobalBounds());
}

EntityType::Type Bullet::getVictim() const {
    if(owner & EntityType::INACTIVE_PLAYER ||
    owner & EntityType::ACTIVE_PLAYER) {
        return EntityType::BOSS;
    }
    return EntityType::PLAYER;
}

void Bullet::use() {
    isUsed = true;
}

int Bullet::getDamage() const {
    return damage;
}

sf::Vector2f Bullet::getCoordinates() {
    return coordinates;
}

void Bullet::drawSprite(sf::RenderWindow *window) {
    window->draw(sprite);
}

sf::Vector2f Bullet::getFacing() {
    return facing;
}
