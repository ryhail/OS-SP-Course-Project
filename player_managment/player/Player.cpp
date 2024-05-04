#include "Player.h"

Player::Player(TextureHolder *textures, Textures::ID playerType)
        : Entity(){
    playerSprite.setTexture(textures->getResource(playerType));
    playerSprite.setPosition(coordinates.x, coordinates.y);
    facing.y = 0;
    facing.x = 0;
    speed = PLAYER_INIT_SPEED;
}

void Player::takeDamage(int dmg) {
    hitPoints-=dmg;
}

void Player::heal(int heal) {
    hitPoints+=heal;
}

void Player::updateFacing(int x, int y) {
    facing.x = x;
    facing.y = y;
}

void Player::setActive(bool value) {
    active = value;
}

bool Player::isActive() const {
    return active;
}

void Player::draw(sf::RenderWindow* window) {
    window->draw(playerSprite);
}

EntityType::Type Player::getCategory() const {
    return active ? EntityType::ACTIVE_PLAYER : EntityType::INACTIVE_PLAYER;
}

int Player::getSpeed() const {
    return speed;
}
/*
 * прибавляет к текущим координатам coordinates смещение
 * смещение считается speed * time в проекциях вектора по Х, У
 * таким образом обеспечивается перемещение по времени
 */
void Player::move(sf::Vector2i direction, sf::Time dt) {
    coordinates.x += direction.x * speed * dt.asSeconds();
    coordinates.y += direction.y * speed * dt.asSeconds();
}

void Player::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(playerSprite, states);
}

