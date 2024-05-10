#include <iostream>
#include "Player.h"
#include "../../command/CommandQueue.h"

Player::Player(TextureHolder* textures, Textures::ID playerType)
        : Entity(){
    playerSprite.setTexture(textures->getResource(playerType));
    playerSprite.setPosition(coordinates.x, coordinates.y);
    playerSprite.setOrigin((playerSprite.getLocalBounds().height/2),playerSprite.getLocalBounds().height/1.24);
    facing.y = 0;
    facing.x = 0;

    isFiring = false;
    fireCountdown = PLAYER_FIRING_INTERVAL;
    fireCommand.category = EntityType::ACTIVE_PLAYER;
    fireCommand.action = [this, textures](SceneNode& node, sf::Time dt) {
        createBullet(node, *textures);
    };

    bulletCount = 5;
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
    sf::Vector2f newCoords;
    newCoords.x = coordinates.x + direction.x * speed * dt.asSeconds();
    newCoords.y = coordinates.y + direction.y * speed * dt.asSeconds();
    if(currentMapTile->getCurrentTileType(newCoords) != Tile::Type::Border) {
        coordinates = newCoords;
        playerSprite.setPosition(newCoords);
    }
}


void Player::setSpritePosition(sf::Vector2f _coords) {
    playerSprite.setPosition(_coords);
}

void Player::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(playerSprite, states);
}

void Player::fire() {
    isFiring = true;
}

void Player::checkBulletLaunch(CommandQueue& commandQueue, sf::Time dt) {
    std::cout << "check for fight!" << std::endl;
    if (firingAvailable()) {
        fireCountdown = PLAYER_FIRING_INTERVAL;
        commandQueue.push(fireCommand);
        isFiring = false;
    } else if (fireCountdown > sf::Time::Zero) {
        fireCountdown -= dt;
        isFiring = false;
    }
}

bool Player::firingAvailable() const {
    return bulletCount > 0 && fireCountdown <= sf::Time::Zero;
}

void Player::createBullet(SceneNode &node, TextureHolder &textures) {
    std::unique_ptr<Bullet> bullet(new Bullet(facing, coordinates, Bullet::Owner::PLAYER, textures));
    node.addChild(std::move(bullet));
    decrementBulletCount();
    std::cout << "bullet created!" << std::endl;
}

void Player::updateCurrent(sf::Time dt, CommandQueue &queue) {
    if(isFiring)
        checkBulletLaunch(queue, dt);
    //Entity::updateCurrent(dt, queue);
}

bool Player::isForRemove(sf::RenderWindow &window) {
    return hitPoints <= 0;
}

void Player::decrementBulletCount() {
    bulletCount -= 1;
}

void Player::setCurentMapTile(MapTile* mapTile) {
    currentMapTile = mapTile;
}

