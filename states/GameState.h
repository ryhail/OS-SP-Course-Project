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

private:
    void    handleCollisions();

private:
    int             sockfd;
    sockaddr_in     server_adr;
    Level           mLevel;
    Player*         controlledPlayer;
    Player*         updatedPlayer;
    SceneNode       sceneGraph;
    CommandQueue    commandQueue;
    InputHandler    inputHandler;
};



#endif //SFML_GAMESTATE_H
