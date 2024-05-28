#ifndef SFML_GAMESTATE_H
#define SFML_GAMESTATE_H

#include "State.h"
#include "../levelDesign/Level.h"
#include "../command/CommandQueue.h"
#include "../input_handler/InputHandler.h"
#include "../entity_managment/Boss.h"

#define PICKUPDELAY sf::seconds(2)

class GameState : public State {
public:
    GameState(StateStack& stack,
    Context context);
    void         buildScene();
    void         draw() override;
    bool         update(sf::Time dt) override;
    bool         handleEvent(const sf::Event& event) override;
    void drawHeart(Entity *entity, sf::RenderWindow* window);

private:
    void    handleCollisions();
    void animateBoss(sf::Time dt); //todo: убрать
private:
    int             sockfd;
    sockaddr_in     server_adr;
    int             seed;
    client_data_t   msgToServer;
    send_data       msgFromServer;
    Level*          mLevel;
    Player*         controlledPlayer;
    Player*         updatedPlayer;
    Boss*           boss;
    SceneNode       sceneGraph;
    CommandQueue    commandQueue;
    InputHandler    inputHandler;
    sf::Time        serverDelay;
    sf::Time        pickupDelay;
    sf::Sprite      heart;
    TextureHolder   textureHolder;
};



#endif //SFML_GAMESTATE_H
