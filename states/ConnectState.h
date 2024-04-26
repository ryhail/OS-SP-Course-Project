#ifndef SFML_CONNECTSTATE_H
#define SFML_CONNECTSTATE_H


#include <SFML/Graphics.hpp>
#include "State.h"

class ConnectState : public State {
public:
    ConnectState(StateStack& stack,
    Context context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);


private:
    sf::RectangleShape mFrame;
    sf::RectangleShape mChoices[2];
    bool               mChoice;
    sf::RectangleShape mConnectButton;
    sf::Text           mConnectText;
};


#endif //SFML_CONNECTSTATE_H
