//
// Created by hvorostok on 19.05.2024.
//

#ifndef SFML_HEART_H
#define SFML_HEART_H


#include <SFML/Graphics/Sprite.hpp>
#include "../entity_managment/SceneNode.h"
#include "../entity_managment/Bullet/Bullet.h"

class Heart : public SceneNode{
public:
    explicit Heart(TextureHolder& textures);
    bool isForRemove() override;

    EntityType::Type getCategory() const override;
    sf::FloatRect getBoundingRect() const override;

    void setPosition(sf::Vector2f position);
    void broke();

private:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    sf::Sprite   sprite;
    sf::Vector2f pos;
    bool         broken;
};


#endif //SFML_HEART_H
