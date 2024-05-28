#ifndef SFML_GAMESTATE_H
#define SFML_GAMESTATE_H

#include "State.h"
#include "../levelDesign/Level.h"
#include "../command/CommandQueue.h"
#include "../input_handler/InputHandler.h"

class GameState : public State {
public:
    GameState(StateStack& stack,
    Context context);
    void         buildScene();
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    void drawHeart(Player* player, sf::RenderWindow* window);
private:
    void    handleCollisions();
    void animateBoss(sf::Time dt); //todo: убрать
private:
    //todo: убрать
    sf::Sprite      boss;
    sf::Time        animationBoss;
    int currentFrame;
    //
    int             sockfd;
    sockaddr_in     server_adr;
    int seed;
    client_data_t   msgToServer;
    game_data_t     msgFromServer;
    Level*          mLevel;
    Player*         controlledPlayer;
    Player*         updatedPlayer;
    SceneNode       sceneGraph;
    CommandQueue    commandQueue;
    InputHandler    inputHandler;
    sf::Time        serverDelay;
    sf::Sprite      heart;
};



#endif //SFML_GAMESTATE_H
