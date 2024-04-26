#ifndef SFML_STATE_H
#define SFML_STATE_H

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include "StateIdentifiers.h"
#include "StateStack.h"

namespace sf
{
    class RenderWindow;
}



class State
{
public:
    typedef std::unique_ptr<State> Ptr;
    struct Context
    {
        Context(sf::RenderWindow& window);
        sf::RenderWindow*	window;
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
