#ifndef SFML_MENUSTATE_H
#define SFML_MENUSTATE_H

#include <SFML/Graphics.hpp>
#include "State.h"
#define MAX_MENU_OPTIONS 3

class MenuState : public State {
public:
    MenuState(StateStack& stack,
    Context context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);



private:
    sf::Sprite          mBackgroundSprite;
    sf::Text            mOptions[MAX_MENU_OPTIONS];
private:
    int                 isCursorOnMenuOption(int x, int y);
};


#endif //SFML_MENUSTATE_H
