#ifndef SFML_GAMESTATE_H
#define SFML_GAMESTATE_H

#include "State.h"
#include "../levelDesign/Level.h"

class GameState : public State {
public:
    GameState(StateStack& stack,
    Context context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
private:
    int             sockfd;
    struct          sockaddr_in;
    Level           mLevel;
    Player*          controlledPlayer;
    Player*          updatedPlayer;
};



#endif //SFML_GAMESTATE_H
