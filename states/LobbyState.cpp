
#include "LobbyState.h"

LobbyState::LobbyState(StateStack &stack, State::Context context)
    : State(stack, context)
    , mChoice(false)
    , readyState(false) {

    sf::Vector2f windowSize = context.window->getView().getSize();

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
}

bool LobbyState::handleEvent(const sf::Event &event) {
    if(!readyState) {
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
    }
    if(mChoice) {
        mFrame.setPosition(mChoices[1].getPosition() - mChoices[1].getSize() / 8.f);
    } else {
        mFrame.setPosition(mChoices[0].getPosition() - mChoices[0].getSize() / 8.f);
    }
    switch(event.key.code) {
        case sf::Keyboard::Key::Enter: {
            readyState = true;
            mReady.setFillColor(sf::Color::Green);
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
    return true;
}
