#include <iostream>
#include <set>
#include "GameState.h"

GameState::GameState(StateStack &stack, State::Context context) : State(stack, context),
    mLevel(context.window)
{
    if(context.player1->isActive()) {
        std::cout << "c1" << std::endl;
        controlledPlayer = context.player1;
        updatedPlayer = context.player2;
    } else if(context.player2->isActive()){
        std::cout << "c2" << std::endl;
        controlledPlayer = context.player2;
        updatedPlayer = context.player1;
    }
    controlledPlayer->update(mLevel.getCurrentMapTile()->getSpawnPoint());
    controlledPlayer->setSpritePosition(mLevel.getCurrentMapTile()->getSpawnPoint());
    controlledPlayer->setCurentMapTile(mLevel.getCurrentMapTile());
    updatedPlayer->update(mLevel.getCurrentMapTile()->getSpawnPoint());
    updatedPlayer->setSpritePosition(mLevel.getCurrentMapTile()->getSpawnPoint());
    updatedPlayer->setCurentMapTile(mLevel.getCurrentMapTile());
    buildScene();
    msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
    msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
}

void GameState::draw() {
    mLevel.draw();
    //controlledPlayer->draw(getContext().window);
    //updatedPlayer->draw(getContext().window);
    getContext().window->draw(sceneGraph);
    //std::cout<<controlledPlayer->getCoordinates().x<<std::endl;
}

bool GameState::update(sf::Time dt) {
    send_client_data(msgToServer, sockfd, server_adr);
    receive_game_data(&msgFromServer, sockfd, server_adr);
    while (!commandQueue.isEmpty()) {
        sceneGraph.execCommand(commandQueue.pop(), dt);
    }
    sceneGraph.update(dt, commandQueue);
    handleCollisions();
    sceneGraph.removeWrecks();
    inputHandler.handleRealtimeInput(commandQueue);
    if(commandQueue.isEmpty())
        controlledPlayer->animate(Idle, dt);
    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
    return true;
}

void GameState::buildScene() {
    SceneNode::SceneNodePtr activePlayer(controlledPlayer);
    sceneGraph.addChild(std::move(activePlayer));
    SceneNode::SceneNodePtr passivePlayer(updatedPlayer);
    sceneGraph.addChild(std::move(passivePlayer));
}
/*
 * обработка столкновений между сущностями
 * (логика выстрела, подбирашек)
 */
void GameState::handleCollisions() {
    std::set<SceneNode::Pair> collidePairs;
    sceneGraph.checkSceneCollision(sceneGraph, collidePairs);

    for (SceneNode::Pair pair : collidePairs) {
        if(hasSpecifiedCategories(pair, EntityType::ACTIVE_PLAYER, EntityType::BULLET)
        || hasSpecifiedCategories(pair, EntityType::INACTIVE_PLAYER, EntityType::BULLET)) {
                auto& player = dynamic_cast<Player&>(*pair.first);
                auto& bullet = dynamic_cast<Bullet&>(*pair.second);
                if (bullet.getVictim() & EntityType::PLAYER) {
                    player.takeDamage(bullet.getDamage());
                    bullet.use();
                }
        }
        //босса еще нет
        if(hasSpecifiedCategories(pair, EntityType::BOSS, EntityType::BULLET)) {
            //auto& boss = dynamic_cast<Boss&>(*pair.first);
            auto& bullet = dynamic_cast<Bullet&>(*pair.second);
            if (bullet.getVictim() & EntityType::BOSS) {
                //boss.takeDamage(bullet.getDamage());
                bullet.use();
            }
        }
        // тестировать, что работает
        if(hasSpecifiedCategories(pair, EntityType::ACTIVE_PLAYER, EntityType::BULLET)
           || hasSpecifiedCategories(pair, EntityType::INACTIVE_PLAYER, EntityType::BULLET)) {
            auto& player = dynamic_cast<Player&>(*pair.first);
            auto& bullet = dynamic_cast<Bullet&>(*pair.second);
            if (bullet.getVictim() & EntityType::BOSS) {
                player.takeDamage(bullet.getDamage());
                bullet.use();
            }
        }
        // подбирашки
    }
}
