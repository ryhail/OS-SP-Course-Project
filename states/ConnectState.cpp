#include "ConnectState.h"
#include "utility.h"
ConnectState::ConnectState(StateStack &stack, State::Context context)
    : State(stack, context)
    , mChoice(false) {
    sf::Vector2f windowSize = context.window->getView().getSize();
    mConnectButton.setSize(sf::Vector2f(windowSize.x / 8, windowSize.y / 16));
    mConnectButton.setFillColor(sf::Color::White);
    mConnectButton.setPosition(windowSize.x / 2.f
                               - mConnectButton.getSize().x / 2,
                               windowSize.y / 1.5f);

    mConnectText.setFont(context.fonts->getResource(Fonts::MainLetters));
    mConnectText.setString("enter");
    mConnectText.setPosition(mConnectButton.getPosition() +
    sf::Vector2f(mConnectText.getLocalBounds().width/4, mConnectText.getLocalBounds().height / 4));
    mConnectText.setFillColor(sf::Color::Red);
    mConnectText.setCharacterSize((int) mConnectButton.getSize().x / 6);

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
}

void ConnectState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mConnectButton);
    window.draw(mConnectText);
    window.draw(mFrame);
    window.draw(mChoices[0]);
    window.draw(mChoices[1]);
}

bool ConnectState::update(sf::Time dt) {
    return true;
}

bool ConnectState::handleEvent(const sf::Event &event) {
    switch (event.key.code) {
        case sf::Keyboard::Key::Left:
            mChoice = false;
            break;
        case sf::Keyboard::Key::Right:
            mChoice = true;
            break;
        case sf::Keyboard::Key::Enter:
            //todo
            break;
        default:
            break;
    }
    if(mChoice) {
        mFrame.setPosition(mChoices[1].getPosition() - mChoices[1].getSize() / 8.f);
    } else {
        mFrame.setPosition(mChoices[0].getPosition() - mChoices[0].getSize() / 8.f);
    }
    return true;
}


