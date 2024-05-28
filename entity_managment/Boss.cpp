//
// Created by hvorostok on 22.05.2024.
//

#include <iostream>
#include "Boss.h"
#include "../states/utility.h"

Boss::Boss(sf::Vector2f _coordinates, int _hitPoints, TextureHolder& textures) {
    textures.load(Textures::Boss, "resources/Textures/boss.png");
    sprite.setTexture(textures.getResource(Textures::Boss));
    sprite.setTextureRect(sf::IntRect(0,0,69,94));
    sprite.setPosition(coordinates);
    centerOrigin(sprite);
    coordinates = _coordinates;
    hitPoints = _hitPoints;
    currentFrame = 0;
    animationDeltaTime = sf::Time::Zero;
}

bool Boss::isForRemove() {
    return hitPoints <= 0;
}

EntityType::Type Boss::getCategory() const {
    return EntityType::BOSS;
}

sf::FloatRect Boss::getBoundingRect() const {
    return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

void Boss::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(sprite, states);
}

void Boss::updateCurrent(sf::Time dt, CommandQueue &queue) {
    animate(dt);
}

void Boss::takeDamage(int dmg) {
    hitPoints-=dmg;
}

sf::Vector2f Boss::getCoordinates() {
    return coordinates;
}

int Boss::getHitPoints() {
    return hitPoints;
}

void Boss::move(sf::Vector2f coordinates) {
    this->coordinates = coordinates;
    sprite.setPosition(coordinates);
}

void Boss::animate(sf::Time dt) {
    animationDeltaTime+=dt;
    if(animationDeltaTime.asSeconds() > 0.15) {
        if(currentFrame == 3)
            currentFrame = 0;
        else
            currentFrame++;
        animationDeltaTime=sf::Time::Zero;
    }
    sprite.setTextureRect(sf::IntRect(currentFrame*69, 0, 69,94));
}