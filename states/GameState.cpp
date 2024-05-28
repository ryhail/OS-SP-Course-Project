#include <iostream>
#include <set>
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


    msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
    msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
    boss = new Boss(*context.textures);
    buildScene();
    textureHolder = *context.textures;
    pickupDelay = PICKUPDELAY;
}

void GameState::draw() {
    mLevel->draw();
    getContext().window->draw(sceneGraph);
    drawHeart(controlledPlayer, getContext().window);
    drawHeart(updatedPlayer, getContext().window);
    drawHeart(boss, getContext().window);
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
    if(serverDelay.asSeconds() > 0.1f) {
        msgToServer.player.coordinates.x = controlledPlayer->getCoordinates().x;
        msgToServer.player.coordinates.y = controlledPlayer->getCoordinates().y;
        bullet_t* last = controlledPlayer->getLastBullet();
        if(last == nullptr) {
            msgToServer.bullet = {0};
        } else msgToServer.bullet = *last;
        send_client_data(msgToServer, sockfd, server_adr);
        serverDelay = sf::Time::Zero;
        receive_game_data(&msgFromServer, sockfd, server_adr);
    }

    std::cout << msgFromServer.player.coordinates.x << ' ' << msgFromServer.player.coordinates.x << std::endl;
    updatedPlayer->setPosition(msgFromServer.player.coordinates.x, msgFromServer.player.coordinates.y);
    boss->setCoordinates(sf::Vector2f (msgFromServer.boss.coordinates.x, msgFromServer.boss.coordinates.y));
    boss->setHitPoints(msgFromServer.boss.hp);
    boss->create();
    while (!commandQueue.isEmpty()) {
        sceneGraph.execCommand(commandQueue.pop(), dt);
    }
    sceneGraph.update(dt, commandQueue);
    handleCollisions();
    sceneGraph.removeWrecks();
    inputHandler.handleRealtimeInput(commandQueue);
    pickupDelay -= dt;
    if(pickupDelay <= sf::Time::Zero) {
        float x = rand() % (780 - 64 + 1) + 64;
        float y = rand() % (1280 - 64 + 1) + 64;
        sf::Vector2f coord(x, y);
        std::unique_ptr<Pickup> pickup(new Pickup(coord, mLevel->getCurrentMapTile(), rand() % 2, textureHolder));
        sceneGraph.addChild(std::move(pickup));
        pickupDelay = PICKUPDELAY;
    }
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
    SceneNode::SceneNodePtr updatedBoss(boss);
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
        if(hasSpecifiedCategories(pair, EntityType::ACTIVE_PLAYER, EntityType::BULLET)
        || hasSpecifiedCategories(pair, EntityType::INACTIVE_PLAYER, EntityType::BULLET)) {
                auto& player = dynamic_cast<Player&>(*pair.first);
                auto& bullet = dynamic_cast<Bullet&>(*pair.second);
                if (bullet.getVictim() & EntityType::PLAYER) {
                    bullet.use();
                }
        }
        if(hasSpecifiedCategories(pair, EntityType::BOSS, EntityType::BULLET)) {
            //auto& boss = dynamic_cast<Boss&>(*pair.first);
            auto& bullet = dynamic_cast<Bullet&>(*pair.second);
            if (bullet.getVictim() & EntityType::BOSS) {
                bullet.use();
            }
        }
        if(hasSpecifiedCategories(pair, EntityType::PLAYER, EntityType::PICKUP)) {
            auto& player = dynamic_cast<Player&>(*pair.first);
            auto& pickup = dynamic_cast<Pickup&>(*pair.second);
            pickup.pickup(player);
            pickup.use();
        }
    }
}

