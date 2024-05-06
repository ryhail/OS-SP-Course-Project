
#include "LobbyState.h"

LobbyState::LobbyState(StateStack &stack, State::Context context)
    : State(stack, context)
    , mChoice(false) {

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

}

void LobbyState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mFrame);
    window.draw(mChoices[0]);
    window.draw(mChoices[1]);
}

bool LobbyState::handleEvent(const sf::Event &event) {
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
    if(mChoice) {
        mFrame.setPosition(mChoices[1].getPosition() - mChoices[1].getSize() / 8.f);
    } else {
        mFrame.setPosition(mChoices[0].getPosition() - mChoices[0].getSize() / 8.f);
    }
    return true;
}

bool LobbyState::update(sf::Time dt) {
    return true;
}
