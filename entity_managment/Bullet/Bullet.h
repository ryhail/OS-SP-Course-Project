#ifndef COURSE_BULLET_H
#define COURSE_BULLET_H

#include <SFML/Graphics/Sprite.hpp>
#include "../Entity/Entity.h"
#include "../../resources/ResourceHolder.h"
#include <SFML/Graphics/RenderTarget.hpp>

typedef ResourceHolder<Textures::ID, sf::Texture> TextureHolder;

class Bullet: public Entity{
    enum Owner {
        PLAYER = 1 << 1,
        BOSS   = 1 << 2
    };
public:
    Bullet(bullet_t bulletInfo, const TextureHolder &bulletTexture);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Sprite sprite;
    Owner owner;
};
#endif //COURSE_BULLET_H
