#ifndef SFML_APPLICATION_H
#define SFML_APPLICATION_H


#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Text.hpp>
#include "states/StateStack.h"

class Application {
public:
    Application();
    void					run();


private:
    void					processInput();
    void					update(sf::Time dt);
    void					render();

    void					updateStatistics(sf::Time dt);
    void					registerStates();


private:
    static const sf::Time	TimePerFrame;

    sf::RenderWindow		mWindow;
    StateStack				mStateStack;
    sf::Font                mStatisticTextFont;
    sf::Text				mStatisticsText;
    sf::Time				mStatisticsUpdateTime;
    std::size_t				mStatisticsNumFrames;
};


#endif //SFML_APPLICATION_H
