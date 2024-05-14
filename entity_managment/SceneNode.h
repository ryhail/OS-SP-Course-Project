#ifndef COURSE_SCENENODE_H
#define COURSE_SCENENODE_H


#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "sever_structures.h"
#include "Entity/EntityType.h"
#include "../command/Command.h"
#include "../command/CommandQueue.h"

class SceneNode: public sf::Transformable,
        public sf::Drawable,
        private sf::NonCopyable {
public:
    typedef std::unique_ptr<SceneNode> SceneNodePtr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
    SceneNode() = default;

    void            addChild(SceneNodePtr child);
    SceneNodePtr    detachChild(const SceneNode& child);
    void            update(sf::Vector2f _coordinate);
    void            update(sf::Time dt, CommandQueue &queue);

    void            execCommand(const Command& command, sf::Time dt);
    void            draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual bool                isForRemove();
    virtual EntityType::Type    getCategory() const;

    sf::Vector2f			getWorldPosition() const;
    sf::Transform			getWorldTransform() const;
    virtual sf::FloatRect	getBoundingRect() const;
    void                    checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
    void                    checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);

private:
    virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void    updateCurrent(sf::Vector2f _coordinate);
    void            updateChildren(sf::Vector2f _coordinate);
    virtual void    updateCurrent(sf::Time dt, CommandQueue &queue);
    void            updateChildren(sf::Time dt, CommandQueue &queue);

private:
    std::vector<SceneNodePtr>   childSceneNodes;
    SceneNode*                  parent;
};

bool	collision(const SceneNode& lhs, const SceneNode& rhs);
bool    hasSpecifiedCategories(SceneNode::Pair collidePair, EntityType::Type first, EntityType::Type second);

#endif //COURSE_SCENENODE_H
