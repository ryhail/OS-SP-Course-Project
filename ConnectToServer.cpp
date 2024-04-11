#include "ConnectToServer.h"

void ConnectToServer::update(sf::Event* event) {
    switch(event->type) {
        case(sf::Event::EventType::TextEntered): {
            if((event->text.unicode <= 57 && event->text.unicode >= 48)
            || event->text.unicode == 46) {
                if(userInput.size() == 20)
                    break;
                userInput += event->text.unicode;
                textField.setString(userInput);
            }
        }
        case(sf::Event::EventType::KeyPressed): {
            if(event->key.code == sf::Keyboard::BackSpace) {
                if(userInput.empty())
                    break;
                userInput.pop_back();
                textField.setString(userInput);
            }
            break;
        }
        case(sf::Event::EventType::KeyReleased): {
            if(event->key.code == sf::Keyboard::Enter) {
                if(userInput.empty())
                    break;

            }
        }
        default:
            break;
    }
}

void ConnectToServer::draw(sf::RenderWindow *window) {
    window->draw(backGround);
    window->draw(textBoard);
    window->draw(textField);
}

ConnectToServer::ConnectToServer() {
    if(!font.loadFromFile("resources/Fonts/PixelifySans.ttf")) {
        exit(1);
    }
    status = true;
    textBoard.setSize(sf::Vector2f(480, 60));
    textBoard.setPosition(400, 220);
    textBoard.setFillColor(sf::Color::White);
    textField.setFillColor(sf::Color::Red);
    textField.setPosition(410, 220);
    textField.setCharacterSize(40);
    textField.setFont(font);
    textField.setString("server ip");
}
