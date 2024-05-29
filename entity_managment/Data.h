//
// Created by hvorostok on 17.05.2024.
//

#ifndef SFML_DATA_H
#define SFML_DATA_H


#include <SFML/Graphics/Texture.hpp>
#include "../player_managment/player/Player.h"

struct PickUpData {
    Textures::ID texture;
    sf::Color   color;
    std::function<void(Player&)> pickupDoing;
};

std::vector<PickUpData> fillPickUp();

#endif //SFML_DATA_H
