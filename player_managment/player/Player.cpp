#include "Player.h"

Player::Player(TextureHolder *textures, Textures::ID playerType) {
    playerSprite.setTexture(textures->getResource(playerType));
    coordinates.y = 200;
    coordinates.x = 200;
    playerSprite.setPosition(coordinates.x, coordinates.y);
    facing.y = 0;
    facing.x = 0;
}

void Player::takeDamage(int dmg) {
    hitPoints-=dmg;
}

void Player::heal(int heal) {
    hitPoints+=heal;
}

void Player::updateCoordinates(sf::Vector2i newCoords) {
    coordinates = newCoords;
    playerSprite.setPosition(coordinates.x, coordinates.y);
}

void Player::updateFacing(int x, int y) {
    facing.x = x;
    facing.y = y;
}

void Player::setActive(bool value) {
    active = value;
}

bool Player::isActive() {
    return active;
}

void Player::draw(sf::RenderWindow* window) {
    window->draw(playerSprite);
}

sf::Vector2i Player::getCoordinates() {
    return coordinates;
}
