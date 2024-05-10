#include <iostream>
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

}

void GameState::draw() {
    mLevel.draw();
    //controlledPlayer->draw(getContext().window);
    //updatedPlayer->draw(getContext().window);
    getContext().window->draw(sceneGraph);
    //std::cout<<controlledPlayer->getCoordinates().x<<std::endl;
}

bool GameState::update(sf::Time dt) {
    std::cout<<mLevel.getCurrentMapTile()->getCurrentTileType(controlledPlayer->getCoordinates())<<std::endl;
    while (!commandQueue.isEmpty()) {
        sceneGraph.execCommand(commandQueue.pop(), dt);
    }
    sceneGraph.update(dt, commandQueue);
    inputHandler.handleRealtimeInput(commandQueue);
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
