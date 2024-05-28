#include <iostream>
#include "Player.h"
#include "../../command/CommandQueue.h"

Player::Player(TextureHolder* textures, Textures::ID playerType)
        : Entity(), mTextures(textures){
    playerSprite.setTexture(textures->getResource(playerType));
    playerSprite.setTextureRect(sf::IntRect (0,0,43,64));
    playerSprite.setPosition(coordinates.x, coordinates.y);
    playerSprite.setOrigin((playerSprite.getLocalBounds().width/2),playerSprite.getLocalBounds().height);

    hitPoints = PLAYER_INIT_HITPOINTS;

    facing.y = 0;
    facing.x = 0;

    firingShift = coordinates;
    isFiring = false;
    fireCountdown = PLAYER_FIRING_INTERVAL;
    fireCommand.category = EntityType::ACTIVE_PLAYER;
    fireCommand.action = [this, textures](SceneNode& node, sf::Time dt) {
        createBullet(node, *textures);
    };
    surfaceDeltaTime = sf::Time::Zero;
    animationDeltaTime = sf::Time::Zero;
    animationFrame = 0;
    lastBulletCreated = nullptr;
    bulletCount = 5;
    speed = PLAYER_INIT_SPEED;
}

void Player::takeDamage(int dmg) {
    hitPoints-=dmg;
}

void Player::heal() {
    hitPoints+=1;
}

void Player::updateFacing(float x, float y) {
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
    if(isDead()) return;
    sf::Vector2f newCoords;
    newCoords.x = coordinates.x + direction.x * speed * dt.asSeconds();
    newCoords.y = coordinates.y + direction.y * speed * dt.asSeconds();
    if(currentMapTile->getCurrentTileType(newCoords) != Tile::Type::Border) {
        setPosition(newCoords.x, newCoords.y);
        if(direction.x == -1)
            animate(Left, dt);
        else if(direction.x == 1)
            animate(Right, dt);
        else if(direction.y == -1)
            animate(Up, dt);
        else if(direction.y == 1)
            animate(Down, dt);
    }
}

void Player::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(playerSprite, states);
}

void Player::fire() {
    isFiring = true;
}

void Player::checkBulletLaunch(CommandQueue& commandQueue, sf::Time dt) {
    if (firingAvailable() && isFiring) {
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
   // std::unique_ptr<Bullet> bullet(new Bullet(facing, coordinates + firingShift, getCategory(), textures, currentMapTile));
    //node.addChild(std::move(bullet));
    sf::Vector2f coords = coordinates + firingShift;
    decrementBulletCount();
    lastBulletCreated = new bullet_t {{coords.x, coords.y},
                                      {facing.x, facing.y},
                                      'p'};
}


bullet_t Player::getLastBullet() {
    if(lastBulletCreated == nullptr)
        return {0};
    bullet_t buf = *lastBulletCreated;
    free(lastBulletCreated);
    lastBulletCreated = nullptr;
    return buf;
}

void Player::updateCurrent(sf::Time dt, CommandQueue &queue) {
    if(isDead()) return;
    updateSurface(dt);
    checkBulletLaunch(queue, dt);
}

bool Player::isForRemove() {
    return false;
}

void Player::decrementBulletCount() {
    bulletCount -= 1;
}

void Player::setCurentMapTile(MapTile* mapTile) {
    currentMapTile = mapTile;
}

void Player::animate(Animation AnimType, sf::Time dt) {
    animationDeltaTime+=dt;
    currentAnimation = AnimType;
    if(animationDeltaTime.asSeconds() > 0.15) {
        if(animationFrame == 0)
            animationFrame++;
        else
            animationFrame--;
        animationDeltaTime=sf::Time::Zero;
    }
    if(AnimType == Right)
        playerSprite.setScale(-1,1);
    else
        playerSprite.setScale(1,1);
    switch(AnimType) {
        case Idle:
            playerSprite.setTextureRect(sf::IntRect(0,0,43,64));
            break;
        case Left:
            playerSprite.setTextureRect(sf::IntRect (animationFrame*43,64,43,64));
            break;
        case Right:
            playerSprite.setTextureRect(sf::IntRect (animationFrame*43,64,43,64));
            break;
        case Up:
        case Down:
            playerSprite.setTextureRect(sf::IntRect(animationFrame*43, 128, 43,64));
            break;
    }
}

void Player::setFiringShift(float shiftX, float shiftY) {
    firingShift = sf::Vector2<float>(shiftX, shiftY);;
}

sf::Rect<float> Player::getPlayerSpriteSize() const {
    return playerSprite.getLocalBounds();
}

sf::FloatRect Player::getBoundingRect() const {
    return getWorldTransform()
            .transformRect(playerSprite.getGlobalBounds());
}

void Player::setPosition(float x, float y) {
    coordinates.x = x;
    coordinates.y = y;
    playerSprite.setPosition(x, y);
}

void Player::setPosition(sf::Vector2f pos) {
    coordinates = pos;
    playerSprite.setPosition(pos);
}

int Player::getHitPoints() const {
    return hitPoints;
}

void Player::takeBullets(int bullets) {
    bulletCount += bullets;
}

bool Player::isDead() {
    return hitPoints <= 0;
}

void Player::updateSurface(sf::Time dt) {
    surfaceDeltaTime += dt;
    Tile::Type tileType = currentMapTile->getCurrentTileType(coordinates);
    if(surfaceDeltaTime.asSeconds() > 0.5f) {
        switch (tileType) {
            case (Tile::Field):
                speed = 100;
                break;
            case (Tile::Ice):
                speed = 200;
                break;
            case (Tile::Slime):
                speed = 50;
                break;
            case (Tile::Spades):
                if(surfaceDeltaTime.asSeconds() > 0.6f)
                    takeDamage(1);
                else
                    return;
                break;
        }
        surfaceDeltaTime = sf::Time::Zero;
    }
}

sf::Vector2f Player::getCoordinates() {
    return coordinates;
}

int Player::getHitPoints() {
    return hitPoints;
}

Animation Player::getCurrentAnimation() {
    return currentAnimation;
}

void Player::setCurrentAnimation(Animation animType) {
    currentAnimation = animType;
}

