#include <iostream>
#include "GameState.h"
#include "../server/client.h"
GameState::GameState(StateStack &stack, State::Context context) : State(stack, context),
    mLevel(context.window)
{
    if(context.player1->isActive()) {
        controlledPlayer = context.player1;
        updatedPlayer = context.player2;
    } else {
        controlledPlayer = context.player2;
        updatedPlayer = context.player1;
    }
    controlledPlayer->update(mLevel.getCurrentMapTile()->getSpawnPoint());
    controlledPlayer->setSpritePosition(mLevel.getCurrentMapTile()->getSpawnPoint());
    updatedPlayer->update(mLevel.getCurrentMapTile()->getSpawnPoint());
    updatedPlayer->setSpritePosition(mLevel.getCurrentMapTile()->getSpawnPoint());
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
    while (!commandQueue.isEmpty()) {
        sceneGraph.execCommand(commandQueue.pop(), dt);
    }
    sceneGraph.update(dt, commandQueue);
    inputHandler.handleRealtimeInput(commandQueue);
    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
//    sf::Vector2i coordinates = controlledPlayer->getCoordinates();
//    if(event.type == sf::Event::EventType::KeyPressed) {
//            switch(event.key.code) {
//                case sf::Keyboard::Key::W:
//                    coordinates.y-=5;
//                    break;
//                case sf::Keyboard::Key::S:
//                    coordinates.y+=5;
//                    break;
//                case sf::Keyboard::Key::D:
//                    coordinates.x+=5;
//                    break;
//                case sf::Keyboard::Key::A:
//                    coordinates.y-=5;
//                    break;
//            }
//    }
//    controlledPlayer->update(coordinates);
    return true;
}

void GameState::buildScene() {
    SceneNode::SceneNodePtr activePlayer(controlledPlayer);
    sceneGraph.addChild(std::move(activePlayer));
    SceneNode::SceneNodePtr passivePlayer(updatedPlayer);
    sceneGraph.addChild(std::move(passivePlayer));
}
