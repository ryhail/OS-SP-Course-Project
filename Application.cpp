#include <SFML/Window/Event.hpp>
#include <sstream>
#include "Application.h"
#include "states/TitleState.h"
#include "states/MenuState.h"
#include "states/ConnectState.h"
#include "states/GameState.h"
#include "states/LobbyState.h"
#include "states/HostState.h"
#include "states/IngameMenuState.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
        : mWindow(sf::VideoMode(1280, 720), "CubHead", sf::Style::Close)
        , mFonts()
        , mTextures()
        , mStateStack(State::Context(mWindow, &mTextures, &mFonts))
        , mStatisticsText()
        , mStatisticsUpdateTime()
        , mStatisticsNumFrames(0)
{
    mWindow.setKeyRepeatEnabled(true);

    mFonts.load(Fonts::MainNumbers, "resources/Fonts/PixelifySans.ttf");
    mFonts.load(Fonts::MainLetters, "resources/Fonts/SuperPixel.ttf");

    mStatisticsText.setFont(mFonts.getResource(Fonts::MainNumbers));
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
        if (event.type == sf::Event::Closed)
            mWindow.close();

        mStateStack.handleEvent(event);

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
        std::stringstream stream;
        stream << mStatisticsNumFrames;
        mStatisticsText.setString("FPS: " + stream.str());
        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<ConnectState>(States::Connect);
    mStateStack.registerState<HostState>(States::Host);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<LobbyState>(States::Lobby);
    mStateStack.registerState<IngameMenuState>(States::InGameMenu);
}