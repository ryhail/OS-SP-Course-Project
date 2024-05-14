#include <set>
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

void SceneNode::update(sf::Time dt, CommandQueue &queue) {
    updateCurrent(dt, queue);
    updateChildren(dt, queue);
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

void SceneNode::updateCurrent(sf::Time dt, CommandQueue &queue) {
    //will be overloaded
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue &queue) {
    for (SceneNodePtr& ptr: childSceneNodes) {
        ptr->update(dt, queue);
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

bool SceneNode::isForRemove() {
    return false;
}
/*
 * получить базис пространства
 * самого базового объекта на поле (самого поля)
 */
sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}
/*
 * sf::Transform - матрица перехода
 * к измененному базису
 * например, rotate изменяет положение спрайта в пространстве
 */
sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node != nullptr; node = node->parent)
        transform = node->getTransform() * transform;

    return transform;
}

sf::FloatRect SceneNode::getBoundingRect() const {
    return {};
}
/*
 * проверка на коллизии (столкновения)
 * одной ноды с нодами всего графа
 */
void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs) {
    if (this != &node && collision(*this, node)
        && !isForRemove() && !node.isForRemove()) {
        collisionPairs.insert(std::minmax(this, &node));
    }
    for (SceneNodePtr& child: childSceneNodes) {
        child->checkNodeCollision(node, collisionPairs);
    }
}
/*
 * проверка на коллизии (столкновения)
 * внутри всего графа (каждой ноды с каждой нодой)
 */
void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs) {
    checkNodeCollision(sceneGraph, collisionPairs);
    for (SceneNodePtr & child: sceneGraph.childSceneNodes)
    checkSceneCollision(*child, collisionPairs);
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}
bool hasSpecifiedCategories(SceneNode::Pair collidePair, EntityType::Type first, EntityType::Type second) {
    unsigned int realFirst = collidePair.first->getCategory();
    unsigned int realSecond = collidePair.second->getCategory();

    if (realFirst & first && realSecond & second) {
        return true;
    }
    if (realFirst & second && realSecond & first) {
        std::swap(collidePair.first, collidePair.second);
        return true;
    }
    return false;
}