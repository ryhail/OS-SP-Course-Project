#include <SFML/Window/Event.hpp>
#include "Application.h"
const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
        : mWindow(sf::VideoMode(1280, 720), "States", sf::Style::Close)
        , mStateStack(State::Context(mWindow))
        , mStatisticsText()
        , mStatisticsUpdateTime()
        , mStatisticsNumFrames(0)
{
    mWindow.setKeyRepeatEnabled(false);

    mStatisticTextFont.loadFromFile("resources/Fonts/SuperPixel.ttf");

    mStatisticsText.setFont(mStatisticTextFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10u);

    registerStates();
    mStateStack.pushState(States::Title);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call
            if (mStateStack.isEmpty())
                mWindow.close();
        }

        updateStatistics(dt);
        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);

    mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;
    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        char* fps;
        strfromf64x(fps, 5, "%lu", mStatisticsNumFrames);
        mStatisticsText.setString("FPS: " + std::string(fps));

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Application::registerStates()
{

}