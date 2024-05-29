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
    controlledPlayer->setPosition(mLevel->getCurrentMapTile()->getSpawnPoint());
    controlledPlayer->setCurentMapTile(mLevel->getCurrentMapTile());
    updatedPlayer->setPosition(mLevel->getCurrentMapTile()->getSpawnPoint());
    updatedPlayer->setCurentMapTile(mLevel->getCurrentMapTile());

    bossEntity= new Boss(sf::Vector2f(400.f, 200.f), 5, *context.textures);
    buildScene();
    msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
    msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
    msgToServer.bullet = {0};
}

void GameState::draw() {
    mLevel->draw();
    getContext().window->draw(sceneGraph);
    drawBullets(msgFromServer);
    controlledPlayer->drawHearts(getContext().window);
    updatedPlayer->drawHearts(getContext().window);

    //drawHeart(controlledPlayer, getContext().window);
    //drawHeart(updatedPlayer, getContext().window);
}
void GameState::drawHeart(Entity *entity, sf::RenderWindow* window) {
    sf::Vector2f pos = entity->getCoordinates();
    pos.x -= entity->getBoundingRect().width / 1.25f;
    pos.y -= entity->getBoundingRect().height / 0.8f;
    int hp = entity->getHitPoints();
    for(int i = 0; i < hp; i++) {
        heart.setPosition(pos);
        window->draw(heart);
        pos.x += 15;
    }
}

bool GameState::update(sf::Time dt) {
    serverDelay += dt;
    if(serverDelay.asSeconds() >= 0.0f) {
        msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
        msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
        msgToServer.player.animation = controlledPlayer->getCurrentAnimation();
        msgToServer.player.hp = controlledPlayer->getHitPoints();

        msgToServer.bullet = controlledPlayer->getLastBullet();

        send_client_data(msgToServer, sockfd, server_adr);
        serverDelay = sf::Time::Zero;
        receive_game_data(&msgFromServer, sockfd, server_adr);
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
    //handleCollisions();
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
    if((controlledPlayer->getHitPoints() == 0) && (updatedPlayer->getHitPoints() == 0)) {
        endGameSprite.setTexture(getContext().textures->getResource(Textures::endGameLose));
        endGameSprite.setPosition(0,0);
        getContext().window->draw(endGameSprite);
        sleep(5);
        requestStackPop();
        requestStackPush(States::Menu);
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
