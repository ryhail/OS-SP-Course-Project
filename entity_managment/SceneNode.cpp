#include "SceneNode.h"

void SceneNode::addChild(SceneNode::SceneNodePtr child) {
    child->parent = this;
    childSceneNodes.push_back(std::move(child));
}

SceneNode::SceneNodePtr SceneNode::detachChild(const SceneNode &child) {
    auto foundChild = std::find_if(
            childSceneNodes.begin(), childSceneNodes.end(),
            [&] (SceneNodePtr& ptr) -> bool {
                return ptr.get() == &child;
    });
    assert(foundChild != childSceneNodes.end());
    SceneNodePtr deletedChild = std::move(*foundChild);
    deletedChild->parent = nullptr;
    childSceneNodes.erase(foundChild);
    return deletedChild;
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    drawCurrent(target, states);

    for(const SceneNodePtr& ptr: childSceneNodes) {
        ptr->draw(target, states);
    }
}

void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    //  will be overloaded
}

void SceneNode::update(coordinate _coordinate) {
    updateCurrent(_coordinate);
    updateChildren(_coordinate);
}
void SceneNode::updateCurrent(coordinate _coordinate) {
    //  will be overloaded
}
void SceneNode::updateChildren(coordinate _coordinate) {
    for (SceneNodePtr& ptr: childSceneNodes) {
        ptr->update(_coordinate);
    }
}
