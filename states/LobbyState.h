//
// Created by egorz on 05.05.24.
//

#ifndef SFML_LOBBYSTATE_H
#define SFML_LOBBYSTATE_H


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window.hpp>
#include "State.h"

class LobbyState : public State {
public:
    LobbyState(StateStack& stack,
    Context context);
    virtual void            draw();
    virtual bool            update(sf::Time dt);
    virtual bool            handleEvent(const sf::Event& event);

private:
    sf::RectangleShape      mFrame;
    sf::RectangleShape      mChoices[2];
    bool                    mChoice;

};

#endif //SFML_LOBBYSTATE_H
