#include "MenuState.h"

MenuState::MenuState(StateStack &stack, State::Context context)
    : State(stack, context)
    , mOptions()
    , mBackgroundSprite()
{
    mBackgroundSprite.setTexture(context.textures->getResource(Textures::MainMenu));
    float y_shift = 0;
    for(sf::Text& option: mOptions) {
        option.setFillColor(sf::Color::White);
        option.setFont(context.fonts->getResource(Fonts::MainLetters));
        option.setCharacterSize(50);
        option.setPosition(context.window->getView().getSize() / 2.5f + sf::Vector2f(50, y_shift));
        y_shift+=100;
    }
    mOptions[0].setString("PLAY");
    mOptions[1].setString("EXIT");
}

void MenuState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mBackgroundSprite);
    for(const auto& option: mOptions) {
        window.draw(option);
    }
}

bool MenuState::update(sf::Time dt) {
    return true;
}

bool MenuState::handleEvent(const sf::Event &event) {
    switch(event.type) {
        case(sf::Event::MouseMoved): {
            int currentOption = isCursorOnMenuOption(event.mouseMove.x, event.mouseMove.y);
            if(currentOption == -1) {
                for (auto& menuOption : mOptions)
                    menuOption.setFillColor(sf::Color::White);
                break;
            }
            mOptions[currentOption].setFillColor(sf::Color::Red);
            break;
        }
        case (sf::Event::EventType::MouseButtonReleased): {
            if(event.mouseButton.button != sf::Mouse::Button::Left)
                break;
            int currentOption = isCursorOnMenuOption(event.mouseButton.x, event.mouseButton.y);
            switch(currentOption) {
                case 0:
                    break;
                case 1:
                    requestStackPop();
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }

    return true;
}

int MenuState::isCursorOnMenuOption(int x, int y) {
    sf::Vector2f mousePos((float )x, (float )y);
    bool isOnFlag = false;
    int currentMenuOption = 0;
    while(currentMenuOption < MAX_MENU_OPTIONS) {
        if (mousePos.x > mOptions[currentMenuOption].getPosition().x &&
            mousePos.x < mOptions[currentMenuOption].getPosition().x
                         + mOptions[currentMenuOption].getGlobalBounds().getSize().x) {
            if (mousePos.y > mOptions[currentMenuOption].getPosition().y &&
                mousePos.y < mOptions[currentMenuOption].getPosition().y
                             + mOptions[currentMenuOption].getGlobalBounds().getSize().y) {
                isOnFlag = true;
            } else {
                currentMenuOption++;
            }
        } else {
            currentMenuOption++;
        }
        if(isOnFlag)
            return currentMenuOption;
    }
    return -1;
}