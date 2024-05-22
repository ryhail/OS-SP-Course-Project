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
    sf::Vector2f pos(10.f, 10.f);
   for(int i = 0; i < PLAYER_INIT_HITPOINTS; i++) {
        std::unique_ptr<Heart> health(new Heart(*textures));
        healthForDisplay.push_back(health.get());
        addChild(std::move(health));
    }
    updateHealthDisplay();

    facing.y = 0;
    facing.x = 0;

    firingShift = coordinates;
    isFiring = false;
    fireCountdown = PLAYER_FIRING_INTERVAL;
    fireCommand.category = EntityType::ACTIVE_PLAYER;
    fireCommand.action = [this, textures](SceneNode& node, sf::Time dt) {
        createBullet(node, *textures);
    };

    animationDeltaTime = sf::Time::Zero;
    animationFrame = 0;

    bulletCount = 5;
    speed = PLAYER_INIT_SPEED;
}


void Player::updateHealthDisplay() {
    sf::Vector2f pos(getCoordinates().x - getBoundingRect().width / 1.25f, getCoordinates().y - getBoundingRect().height / 0.8f);
    for(auto & heart : healthForDisplay) {
        heart->setPosition(pos);
        pos.x+=15.f;
    }
}

void Player::takeDamage(int dmg) {
    for(int i = 0; i < dmg; i++) {
        healthForDisplay.back()->broke();
        healthForDisplay.pop_back();
    }
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
    std::unique_ptr<Bullet> bullet(new Bullet(facing, coordinates + firingShift, getCategory(), textures, currentMapTile));
    node.addChild(std::move(bullet));
    decrementBulletCount();
    std::cout << "bullet created!" << std::endl;
}

void Player::updateCurrent(sf::Time dt, CommandQueue &queue) {
    if(isDead()) return;
    checkBulletLaunch(queue, dt);
    updateHealthDisplay();
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
