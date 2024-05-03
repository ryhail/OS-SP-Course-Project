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
#include "sever_structures.h"
#include "Entity/EntityType.h"
#include "../command/Command.h"

class SceneNode: public sf::Transformable,
        public sf::Drawable,
        private sf::NonCopyable {
public:
    typedef std::unique_ptr<SceneNode> SceneNodePtr;

public:
    SceneNode() = default;

    void            addChild(SceneNodePtr child);
    SceneNodePtr    detachChild(const SceneNode& child);
    void            update(sf::Vector2i _coordinate);
    void            update(sf::Time dt);

    void            execCommand(const Command& command, sf::Time dt);

    virtual EntityType::Type getCategory() const;

private:
    void            draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    virtual void    updateCurrent(sf::Vector2i _coordinate);
    void            updateChildren(sf::Vector2i _coordinate);
    virtual void    updateCurrent(sf::Time dt);
    void            updateChildren(sf::Time dt);


private:
    std::vector<SceneNodePtr>   childSceneNodes;
    SceneNode*                  parent;
};

#endif //COURSE_SCENENODE_H
