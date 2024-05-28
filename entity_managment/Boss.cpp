//
// Created by hvorostok on 22.05.2024.
//

#include <iostream>
#include "Boss.h"
#include "../states/utility.h"

Boss::Boss(sf::Vector2f _coordinates, int _hitPoints, TextureHolder& textures) {
    sprite.setTexture(textures.getResource(Textures::Boss));
    sprite.setPosition(_coordinates);
    centerOrigin(sprite);
    coordinates = _coordinates;
    hitPoints = _hitPoints;
    for(int i = 0; i < hitPoints; i++) {
        std::unique_ptr<Heart> health(new Heart(textures));
        healthForDisplay.push_back(health.get());
        this->addChild(std::move(health));
    }
    updateHealthDisplay();
}

void Boss::updateHealthDisplay() {
    std::cout << "heart updated!" << std::endl;
    sf::Vector2f pos(getCoordinates().x - getBoundingRect().width / 1.25f, getCoordinates().y - getBoundingRect().height / 0.8f);
    for(auto & heart : healthForDisplay) {
        heart->setPosition(pos);
        pos.x+=15.f;
    }
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
    updateHealthDisplay();
}

void Boss::takeDamage(int dmg) {
    for(int i = 0; i < dmg; i++) {
        healthForDisplay.back()->broke();
        healthForDisplay.pop_back();
    }
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
