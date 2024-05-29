//
// Created by hvorostok on 17.05.2024.
//

#include "Data.h"
#include "PickUp/Pickup.h"

std::vector<PickUpData> fillPickUp() {
    std::vector<PickUpData> data(Pickup::Type::TypeCount);

    data[Pickup::Type::HealthRefill].texture = Textures::Heart;
    data[Pickup::Type::HealthRefill].color = sf::Color::Green;
    data[Pickup::Type::HealthRefill].pickupDoing = [](Player& player) { player.heal(); };

    data[Pickup::Type::BulletRefill].texture = Textures::PickUpIncreaseBullet;
    data[Pickup::Type::HealthRefill].color = sf::Color::Blue;
    data[Pickup::Type::BulletRefill].pickupDoing = [](Player& player) { player.takeBullets(1); };
    return data;
}
