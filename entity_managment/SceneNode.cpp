#include <cstdio>
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

void SceneNode::update(sf::Time dt) {
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::update(sf::Vector2f _coordinate) {
    updateCurrent(_coordinate);
    updateChildren(_coordinate);
}
void SceneNode::updateCurrent(sf::Vector2f _coordinate) {
    //  will be overloaded
}
void SceneNode::updateChildren(sf::Vector2f _coordinate) {
    for (SceneNodePtr& ptr: childSceneNodes) {
        ptr->update(_coordinate);
    }
}

EntityType::Type SceneNode::getCategory() const{
    return EntityType::SCENE;
}

void SceneNode::updateCurrent(sf::Time dt) {
    //will be overloaded
}

void SceneNode::updateChildren(sf::Time dt) {
    for (SceneNodePtr& ptr: childSceneNodes) {
        ptr->update(dt);
    }
}

void SceneNode::execCommand(const Command &command, sf::Time dt) {
    if(command.category & getCategory()) {
        command.action(*this, dt);
    }
    for(SceneNodePtr& child : childSceneNodes) {
        child->execCommand(command, dt);
    }
}
