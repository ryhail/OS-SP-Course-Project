#include <iostream>
#include <set>
#include <csignal>
#include "GameState.h"
#include "../entity_managment/PickUp/Pickup.h"

GameState::GameState(StateStack &stack, State::Context context) : State(stack, context)
{
    sockfd = *context.sockfd;
    server_adr = *context.server_adr;
    serverDelay = sf::Time::Zero;
    recv(sockfd,&seed,sizeof(seed),0);
    mLevel = new Level(context.window, seed);
    endGame = false;
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
    controlledPlayer->setPosition(mLevel->getCurrentMapTile()->getSpawnPoint());
    controlledPlayer->setCurentMapTile(mLevel->getCurrentMapTile());
    updatedPlayer->setPosition(mLevel->getCurrentMapTile()->getSpawnPoint());
    updatedPlayer->setCurentMapTile(mLevel->getCurrentMapTile());
    endGameSprite.setPosition(0,0);
    bossEntity= new Boss(sf::Vector2f(400.f, 200.f), 5, *context.textures);
    buildScene();
    msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
    msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
    msgToServer.bullet = {0};
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    heal = false;
}

void GameState::draw() {
    if(endGame) {
        getContext().window->draw(endGameSprite);
    }
    else {
        mLevel->draw();
        getContext().window->draw(sceneGraph);
        drawBullets(msgFromServer);
        controlledPlayer->drawHearts(getContext().window);
        updatedPlayer->drawHearts(getContext().window);
    }
}

bool GameState::update(sf::Time dt) {
    serverDelay += dt;
    if(!endGame) {
        msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
        msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
        msgToServer.player.animation = controlledPlayer->getCurrentAnimation();
        msgToServer.player.hp = controlledPlayer->getHitPoints();
        if (heal){
            msgToServer.heal = 1;
            heal = false;
        }
        if (controlledPlayer->spike){
            msgToServer.heal -= 1;
            controlledPlayer->spike = false;
        }
        msgToServer.bullet = controlledPlayer->getLastBullet();
        send_client_data(msgToServer, sockfd, server_adr);
        serverDelay = sf::Time::Zero;
        receive_game_data(&msgFromServer, sockfd, server_adr);
        msgToServer.heal = 0;
        //if(msgFromServer.hp < controlledPlayer->getHitPoints())
        controlledPlayer->takeDamage(controlledPlayer->getHitPoints() - msgFromServer.hp);

        updatedPlayer->setPosition(sf::Vector2f(msgFromServer.player.coordinates.x, msgFromServer.player.coordinates.y));

        updatedPlayer->setCurrentAnimation(static_cast<Animation>(msgFromServer.player.animation));
        std::cout <<(int) updatedPlayer->getHitPoints() << std::endl;

        updatedPlayer->takeDamage(updatedPlayer->getHitPoints() - msgFromServer.player.hp);
        std::cout <<(int) updatedPlayer->getHitPoints() << std::endl;
        bossEntity->move(sf::Vector2f (msgFromServer.boss.coordinates.x, msgFromServer.boss.coordinates.y));
        bossEntity->takeDamage(bossEntity->getHitPoints() - msgFromServer.boss.hp);
    }
    updatedPlayer->animate(updatedPlayer->getCurrentAnimation(), dt);
    while (!commandQueue.isEmpty()) {
        sceneGraph.execCommand(commandQueue.pop(), dt);
    }
    sceneGraph.update(dt, commandQueue);
    handleCollisions();
    sceneGraph.removeWrecks();
    inputHandler.handleRealtimeInput(commandQueue);
    if(commandQueue.isEmpty() && !controlledPlayer->isForRemove())
        controlledPlayer->animate(Idle, dt);

    if((controlledPlayer->getHitPoints() == 0) && (updatedPlayer->getHitPoints() == 0)) {
        endGameSprite.setTexture(getContext().textures->getResource(Textures::endGameLose));
        endGame = true;
    }
    if(bossEntity->getHitPoints() == 0) {
        endGameSprite.setTexture(getContext().textures->getResource(Textures::endGameWin));
        endGame = true;
    }
    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
    if(event.type == sf::Event::KeyReleased) {
        if(event.key.code == sf::Keyboard::Escape) {
            requestStackPush(States::InGameMenu);
        }
        if(event.key.code == sf::Keyboard::Enter && endGame) {
            requestStackPop();
            requestStackPush(States::Menu);
        }
    }
    return true;
}

void GameState::buildScene() {
    SceneNode::SceneNodePtr activePlayer(controlledPlayer);
    sceneGraph.addChild(std::move(activePlayer));
    SceneNode::SceneNodePtr passivePlayer(updatedPlayer);
    sceneGraph.addChild(std::move(passivePlayer));
    SceneNode::SceneNodePtr updatedBoss(bossEntity);
    sceneGraph.addChild(std::move(updatedBoss));
}
/*
 * обработка столкновений между сущностями
 * (логика выстрела, подбирашек)
 */
void GameState::handleCollisions() {
    std::set<SceneNode::Pair> collidePairs;
    sceneGraph.checkSceneCollision(sceneGraph, collidePairs);

    for (SceneNode::Pair pair : collidePairs) {
//        if(hasSpecifiedCategories(pair, EntityType::ACTIVE_PLAYER, EntityType::BULLET)
//        || hasSpecifiedCategories(pair, EntityType::INACTIVE_PLAYER, EntityType::BULLET)) {
//                auto& player = dynamic_cast<Player&>(*pair.first);
//                auto& bullet = dynamic_cast<Bullet&>(*pair.second);
//                if (bullet.getVictim() & EntityType::PLAYER) {
//                    player.takeDamage(bullet.getDamage());
//                    bullet.use();
//                }
//        }
        if(hasSpecifiedCategories(pair, EntityType::PLAYER, EntityType::PICKUP)) {
            auto& player = dynamic_cast<Player&>(*pair.first);
            auto& pickup = dynamic_cast<Pickup&>(*pair.second);
            pickup.pickup(player);
            if(pickup.getPickUpType() == Pickup::Type::HealthRefill)
                heal = true;
            pickup.use();
        }
//        // тестировать, что работает
//        if(hasSpecifiedCategories(pair, EntityType::ACTIVE_PLAYER, EntityType::BULLET)
//           || hasSpecifiedCategories(pair, EntityType::INACTIVE_PLAYER, EntityType::BULLET)) {
//            auto& player = dynamic_cast<Player&>(*pair.first);
//            auto& bullet = dynamic_cast<Bullet&>(*pair.second);
//            if (bullet.getVictim() & EntityType::BOSS) {
//                player.takeDamage(bullet.getDamage());
//                bullet.use();
//            }
//        }
    }
}

void GameState::drawBullets(send_data &game_data) {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(game_data.new_bullets[i].owner != 0) {
            Bullet bull = Bullet(game_data.new_bullets[i], getContext().textures);
            bull.drawSprite(getContext().window);
        }
    }
}

