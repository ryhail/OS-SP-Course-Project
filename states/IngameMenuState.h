//
// Created by egorz on 25.05.24.
//

#ifndef SFML_INGAMEMENUSTATE_H
#define SFML_INGAMEMENUSTATE_H


#include <SFML/Graphics.hpp>
#include "State.h"
#define MAX_MENU_OPTIONS 2

class IngameMenuState : public State {
public:
    IngameMenuState(StateStack& stack,
              Context context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);



private:
    sf::Text            mOptions[MAX_MENU_OPTIONS];
private:
    int                 isCursorOnMenuOption(int x, int y);
};


#endif //SFML_INGAMEMENUSTATE_H
