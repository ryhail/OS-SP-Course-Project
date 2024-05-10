#ifndef SFML_HOSTSTATE_H
#define SFML_HOSTSTATE_H


#include "State.h"

class HostState : public State{
public:
    HostState(StateStack& stack,
    Context context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
};


#endif //SFML_HOSTSTATE_H
