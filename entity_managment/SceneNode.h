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

class SceneNode: public sf::Transformable,
        public sf::Drawable,
        private sf::NonCopyable {
public:
    typedef std::unique_ptr<SceneNode> SceneNodePtr;

public:
    SceneNode() = default;

    void            addChild(SceneNodePtr child);
    SceneNodePtr    detachChild(const SceneNode& child);
    void            update(coordinate _coordinate);

private:
    void            draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    virtual void    updateCurrent(coordinate _coordinate);
    void            updateChildren(coordinate _coordinate);


private:
    std::vector<SceneNodePtr>   childSceneNodes;
    SceneNode*                  parent;
};

#endif //COURSE_SCENENODE_H
