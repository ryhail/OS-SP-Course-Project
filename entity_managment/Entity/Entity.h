#ifndef COURSE_ENTITY_H
#define COURSE_ENTITY_H


#include <SFML/System/Vector2.hpp>
#include "../sever_structures.h"
#include "../../resources/Textures.h"
#include "../SceneNode.h"

class Entity: public SceneNode{
protected:
    sf::Vector2i coordinates;
public:
    Entity();
    explicit Entity(const sf::Vector2i _coordinates);

    sf::Vector2i getCoordinates() { return coordinates; }

private:
    void updateCurrent(sf::Vector2i _coordinates) override;
    void updateCurrent(sf::Time dt) override;

    //virtual Textures::ID  getResource() = 0;
};
#endif //COURSE_ENTITY_H
