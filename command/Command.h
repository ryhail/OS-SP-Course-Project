#ifndef COURSE_COMMAND_H
#define COURSE_COMMAND_H

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>
#include "../entity_managment/sever_structures.h"
#include "../entity_managment/Entity/EntityType.h"


class SceneNode;

struct Command
{
    typedef std::function<void(SceneNode&, sf::Time dt)> Action;

    Command(): action(), category(EntityType::NONE) { }

    Action						action;
    unsigned int				category;
};
/*
 * функция (действие Action), что может быть вызвана(о) непосредственно на любом объекте,
 * наследующимся от SceneNode.
 * так как Command параметром принимает именно SceneNode&,
 * мы были бы вынуждены каждый раз делать приведение типа.
 * такая сигнатура функции дает непосредственно вызывать на Player, Bullet, ...
 */
template <typename GameObject, typename Function>
Command::Action derivedAction(Function fn){
    return [=] (SceneNode& node, sf::Time dt)
    {
        // Check if cast is safe
        assert(dynamic_cast<GameObject*>(&node) != nullptr);

        // Downcast node and invoke function on it
        fn(static_cast<GameObject&>(node), dt);
    };
}

#endif //COURSE_COMMAND_H
