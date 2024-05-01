#ifndef COURSE_ENTITY_H
#define COURSE_ENTITY_H


#include <SFML/System/Vector2.hpp>
#include "../sever_structures.h"
#include "../../resources/Textures.h"
#include "../SceneNode.h"

class Entity: public SceneNode{
    coordinate coordinates;
public:
    Entity();

private:
    void updateCurrent(coordinate _coordinate) override;
    //virtual Textures::ID  getResource() = 0;
};
#endif //COURSE_ENTITY_H
