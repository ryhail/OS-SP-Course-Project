
#include <iostream>
#include "LobbyState.h"

LobbyState::LobbyState(StateStack &stack, State::Context context)
    : State(stack, context)
    , mChoice(false)
    , readyState(false)
    , available('0')
    , sockfd(*context.sockfd)
    , server_addr(*context.server_adr)
    , serverDelay(sf::Time::Zero) {
    server_addr = *context.server_adr;
    sf::Vector2f windowSize = context.window->getView().getSize();
    context.textures->load(Textures::PlayerUnavailable, "resources/Textures/unavailable.png");

    unavailable.setTexture(context.textures->getResource(Textures::PlayerUnavailable));

    mChoices[0].setPosition(windowSize.x / 2.f - 150, windowSize.y / 3);
    mChoices[0].setSize(sf::Vector2f(100, 100));
    mChoices[0].setFillColor(sf::Color::Blue);

    mChoices[1].setPosition(windowSize.x / 2.f + 50, windowSize.y / 3);
    mChoices[1].setSize(sf::Vector2f(100, 100));
    mChoices[1].setFillColor(sf::Color::Red);

    mFrame.setFillColor(sf::Color::White);
    mFrame.setOutlineThickness(10);
    mFrame.setOutlineColor(sf::Color::Magenta);
    mFrame.setPosition(mChoices[0].getPosition() - mChoices[0].getSize() / 8.f);
    mFrame.setSize(mChoices[0].getSize() * 1.25f);

    mLobbyText.setFont(context.fonts->getResource(Fonts::MainLetters));
    mLobbyText.setString("Lobby: Choose your color");
    mLobbyText.setFillColor(sf::Color::White);
    mLobbyText.setPosition(windowSize.x / 3.5, windowSize.y / 1.5);

    mReady.setFont(context.fonts->getResource(Fonts::MainLetters));
    mReady.setString("ready");
    mReady.setFillColor(sf::Color::Red);
    mReady.setPosition(windowSize.x / 2.2, windowSize.y / 1.25);
    mReady.setCharacterSize(20);
}

void LobbyState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mReady);
    window.draw(mLobbyText);
    window.draw(mFrame);
    window.draw(mChoices[0]);
    window.draw(mChoices[1]);
    window.draw(unavailable);
}

bool LobbyState::handleEvent(const sf::Event &event) {
    if(available == '0') {
        switch (event.key.code) {
            case sf::Keyboard::Key::Left:
                mChoice = false;
                break;
            case sf::Keyboard::Key::Right:
                mChoice = true;
                break;
            default:
                break;
        }
        if (mChoice) {
            mFrame.setPosition(mChoices[1].getPosition() - mChoices[1].getSize() / 8.f);
        } else {
            mFrame.setPosition(mChoices[0].getPosition() - mChoices[0].getSize() / 8.f);
        }
    } else {
        if(available == '1') {
            unavailable.setPosition(mChoices[0].getPosition() - mChoices[0].getSize() / 8.f);
        } else {
            unavailable.setPosition(mChoices[1].getPosition() - mChoices[1].getSize() / 8.f);
        }
    }
    switch(event.key.code) {
        case sf::Keyboard::Key::Enter: {
            readyState = true;
            mReady.setFillColor(sf::Color::Green);
            if(mChoice)
                available = '2';
            else
                available = '1';
            break;
        }
        case sf::Keyboard::Key::Escape: {
            readyState = false;
            mReady.setFillColor(sf::Color::Red);
            break;
        }
    }
    return true;
}

bool LobbyState::update(sf::Time dt) {
    serverDelay += dt;
    if(serverDelay.asSeconds() > 0.05f) {
        sendto(sockfd, &available, sizeof(available), 0, (const sockaddr *) (&server_addr), sizeof(server_addr));
        recv(sockfd, &available, sizeof(available), 0);
        std::cout << available << std::endl;
        serverDelay = sf::Time::Zero;
    }
    if (available == 'r') {
        requestStackPop();
        requestStackPush(States::Game);
    }
    return true;
}
