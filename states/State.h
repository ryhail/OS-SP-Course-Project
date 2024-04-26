#ifndef SFML_STATE_H
#define SFML_STATE_H

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "StateIdentifiers.h"
#include "../resources/ResourceHolder.h"
#include "../resources/Fonts.h"

namespace sf
{
    class RenderWindow;
}

class StateStack;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;
    struct Context
    {
        Context(sf::RenderWindow& window,
                ResourceHolder<Textures::ID, sf::Texture>* textures,
                ResourceHolder<Fonts::ID, sf::Font>* fonts);
        sf::RenderWindow*	                                window;
        ResourceHolder<Textures::ID, sf::Texture>*        textures;
        ResourceHolder<Fonts::ID, sf::Font>*                 fonts;
    };
public:
    State(StateStack& stack, Context context);
    virtual				~State();
    virtual void		draw() = 0;
    virtual bool		update(sf::Time dt) = 0;
    virtual bool		handleEvent(const sf::Event& event) = 0;


protected:
    void				requestStackPush(States::ID stateID);
    void				requestStackPop();
    void				requestStateClear();
    Context				getContext() const;

private:
    StateStack*			mStack;
    Context				mContext;
};

#endif //SFML_STATE_H
