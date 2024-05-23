#ifndef SFML_TITLESTATE_H
#define SFML_TITLESTATE_H

#include <SFML/Graphics.hpp>
#include "State.h"

class TitleState : public State {
public:
    TitleState(StateStack& stack,
               Context context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);



private:
    sf::Sprite          mBackgroundSprite;
    sf::Text            mText;
    sf::Text            mTitles;
    bool                mShowText;
    sf::Time            mTextEffectTime;
};


#endif //SFML_TITLESTATE_H
