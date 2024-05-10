#ifndef COURSE_ENTITY_H
#define COURSE_ENTITY_H


#include <SFML/System/Vector2.hpp>
#include "../sever_structures.h"
#include "../../resources/Textures.h"
#include "../SceneNode.h"

class Entity: public SceneNode{
protected:
    sf::Vector2f coordinates;
public:
    Entity();
    explicit Entity(sf::Vector2f _coordinates);

    sf::Vector2f getCoordinates() { return coordinates; }

private:
    void updateCurrent(sf::Vector2f _coordinates) override;
    void updateCurrent(sf::Time dt, CommandQueue &queue) override;

    //virtual Textures::ID  getResource() = 0;
};
#endif //COURSE_ENTITY_H
