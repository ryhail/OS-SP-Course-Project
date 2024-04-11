#include "MainMenu.h"

MainMenu::MainMenu() {
    if(!menuFont.loadFromFile("resources/Fonts/SuperPixel.ttf")) {
        exit(1);
    }
    if(!textureBackground.loadFromFile("resources/Textures/menubg.png")) {
        exit(1);
    }

    status = true;

    menuBackground.setTexture(textureBackground);
    menuBackground.setPosition(0,0);

    menuOptions[0].setFillColor(sf::Color::White);
    menuOptions[0].setFont(menuFont);
    menuOptions[0].setString("PLAY");
    menuOptions[0].setCharacterSize(50);
    menuOptions[0].setPosition(550,300);

    menuOptions[1].setFillColor(sf::Color::White);
    menuOptions[1].setFont(menuFont);
    menuOptions[1].setString("EXIT");
    menuOptions[1].setCharacterSize(50);
    menuOptions[1].setPosition(550,400);
}

void MainMenu::draw(sf::RenderWindow* window) {
    window->draw(menuBackground);
    for(auto& menuOption : menuOptions)
        window->draw(menuOption);
}

void MainMenu::update(sf::Event *event) {
    switch(event->type) {
        case(sf::Event::MouseMoved): {
            int currentOption = isCursorOnMenuOption(event->mouseMove.x, event->mouseMove.y);
            if(currentOption == -1) {
                for (auto & menuOption : menuOptions)
                    menuOption.setFillColor(sf::Color::White);
                break;
            }
            menuOptions[currentOption].setFillColor(sf::Color::Red);
            break;
        }
        case (sf::Event::EventType::MouseButtonReleased): {
            if(event->mouseButton.button != sf::Mouse::Button::Left)
                break;
            int currentOption = isCursorOnMenuOption(event->mouseButton.x, event->mouseButton.y);
            switch(currentOption) {
                case 0:
                    changeStatus();
                    break;
                case 1:
                    exit(0);
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }
}

bool MainMenu::isActive() {
    return this->status;
}

int MainMenu::isCursorOnMenuOption(int x, int y) {
    sf::Vector2f mousePos((float )x, (float )y);
    bool isOnFlag = false;
    int currentMenuOption = 0;
    while(currentMenuOption < MAX_MENU_OPTIONS) {
        if (mousePos.x > menuOptions[currentMenuOption].getPosition().x &&
            mousePos.x < menuOptions[currentMenuOption].getPosition().x
            + menuOptions[currentMenuOption].getGlobalBounds().getSize().x) {
            if (mousePos.y > menuOptions[currentMenuOption].getPosition().y &&
                mousePos.y < menuOptions[currentMenuOption].getPosition().y
                  + menuOptions[currentMenuOption].getGlobalBounds().getSize().y) {
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

void MainMenu::changeStatus() {
    this->status = !status;
}
