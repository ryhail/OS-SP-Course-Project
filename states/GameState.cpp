#include <iostream>
#include <set>
#include "GameState.h"
#include "../entity_managment/PickUp/Pickup.h"

GameState::GameState(StateStack &stack, State::Context context) : State(stack, context),
                                                                  mLevel(context.window)
{
    sockfd = *context.sockfd;
    server_adr = *context.server_adr;
    serverDelay = sf::Time::Zero;
    int seed;
    recv(sockfd,&seed,sizeof(seed),0);
    // todo: убрать нахуй
    getContext().textures->load(Textures::Boss, "resources/Textures/boss.png");
    boss.setTexture(getContext().textures->getResource(Textures::Boss));
    boss.setTextureRect(sf::IntRect(0,0,69,94));
    boss.setPosition(800,500);
    currentFrame = 0;
    animationBoss = sf::Time::Zero;
    //
    if(context.player1->isActive()) {
        std::cout << "c1" << std::endl;
        controlledPlayer = context.player1;
        updatedPlayer = context.player2;
        msgToServer.player.type = '1';
    } else if(context.player2->isActive()){
        std::cout << "c2" << std::endl;
        controlledPlayer = context.player2;
        updatedPlayer = context.player1;
        msgToServer.player.type = '2';
    }
    heart.setTexture(context.textures->getResource(Textures::Heart));
    controlledPlayer->setPosition(mLevel.getCurrentMapTile()->getSpawnPoint());
    controlledPlayer->setCurentMapTile(mLevel.getCurrentMapTile());
    updatedPlayer->setPosition(mLevel.getCurrentMapTile()->getSpawnPoint());
    updatedPlayer->setCurentMapTile(mLevel.getCurrentMapTile());
    buildScene();
    msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
    msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
}

void GameState::draw() {
    mLevel.draw();
    getContext().window->draw(sceneGraph);
    getContext().window->draw(boss);
    //drawHeart(controlledPlayer, getContext().window);
    //drawHeart(updatedPlayer, getContext().window);
}
//void GameState::drawHeart(Player* player, sf::RenderWindow* window) {
//    sf::Vector2f pos = player->getCoordinates();
//    pos.x -= player->getBoundingRect().width / 1.25f;
//    pos.y -= player->getBoundingRect().height / 0.8f;
//    int hp = player->getHitPoints();
//    for(int i = 0; i < hp; i++) {
//        heart.setPosition(pos);
//        window->draw(heart);
//        pos.x += 15;
//    }
//}

bool GameState::update(sf::Time dt) {
    serverDelay += dt;
    animateBoss(dt);
//    if(serverDelay.asSeconds() > 0.001f) {
//        msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
//        msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
//        send_client_data(msgToServer, sockfd, server_adr);
//        msgToServer.bullet = {0};
//        serverDelay = sf::Time::Zero;
//        receive_game_data(&msgFromServer, sockfd, server_adr);
//    }
//
//    std::cout << msgFromServer.player1.coordinates.x << ' ' << msgFromServer.player1.coordinates.x << std::endl;
//    updatedPlayer->setPosition(msgFromServer.player1.coordinates.x, msgFromServer.player1.coordinates.y);
    while (!commandQueue.isEmpty()) {
        sceneGraph.execCommand(commandQueue.pop(), dt);
    }
    sceneGraph.update(dt, commandQueue);
    handleCollisions();
    sceneGraph.removeWrecks();
    inputHandler.handleRealtimeInput(commandQueue);
    if(commandQueue.isEmpty() && !controlledPlayer->isForRemove())
        controlledPlayer->animate(Idle, dt);
    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
    if(event.type == sf::Event::KeyReleased) {
        if(event.key.code == sf::Keyboard::Escape) {
            requestStackPush(States::InGameMenu);
        }
    }
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
//        if(hasSpecifiedCategories(pair, EntityType::BOSS, EntityType::BULLET)) {
//            //auto& boss = dynamic_cast<Boss&>(*pair.first);
//            auto& bullet = dynamic_cast<Bullet&>(*pair.second);
//            if (bullet.getVictim() & EntityType::BOSS) {
//                //boss.takeDamage(bullet.getDamage());
//                bullet.use();
//            }
//        }
        if(hasSpecifiedCategories(pair, EntityType::PLAYER, EntityType::PICKUP)) {
            auto& player = dynamic_cast<Player&>(*pair.first);
            auto& pickup = dynamic_cast<Pickup&>(*pair.second);
            pickup.pickup(player);
            pickup.use();
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
    }
}

void GameState::animateBoss(sf::Time dt) {
    animationBoss+=dt;
    if(animationBoss.asSeconds() > 0.15) {
        if(currentFrame == 3)
            currentFrame = 0;
        else
            currentFrame++;
        animationBoss=sf::Time::Zero;
    }
    boss.setTextureRect(sf::IntRect(currentFrame*69, 0, 69,94));
}
