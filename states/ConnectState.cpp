#include <iostream>
#include "ConnectState.h"
#include "utility.h"
#include "../server/client.h"
ConnectState::ConnectState(StateStack &stack, State::Context context)
    : State(stack, context) {
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

    centerOrigin(mEnterIpText);
    mEnterIpText.setFont(context.fonts->getResource(Fonts::MainLetters));
    mEnterIpText.setString("Server IP:");
    mEnterIpText.setPosition(windowSize.x/2.4, windowSize.y/3);
    mEnterIpText.setFillColor(sf::Color::White);
    mEnterIpText.setCharacterSize(30);

    centerOrigin(mServerIp);
    mServerIp.setFont(context.fonts->getResource(Fonts::MainNumbers));
    mServerIp.setString("\t\t\t server ip");
    mServerIp.setCharacterSize((int) mConnectButton.getSize().x / 5);
    mServerIp.setFillColor(sf::Color::White);
    mServerIp.setPosition(mConnectButton.getPosition().x - mConnectButton.getSize().x/2, windowSize.y / 2);
}

void ConnectState::draw() {
    sf::RenderWindow& window = *getContext().window;
    window.draw(mConnectButton);
    window.draw(mEnterIpText);
    window.draw(mConnectText);
    window.draw(mServerIp);
}

bool ConnectState::update(sf::Time dt) {

    return true;
}

bool ConnectState::handleEvent(const sf::Event &event) {
    switch(event.type) {
        case(sf::Event::EventType::TextEntered): {
            if((event.text.unicode <= 57 && event.text.unicode >= 48)
               || event.text.unicode == 46) {
                if(mUserInput.size() == 20)
                    break;
                mUserInput += event.text.unicode;
                mServerIp.setString(mUserInput);
            }
        }
        case(sf::Event::EventType::KeyPressed): {
            if(event.key.code == sf::Keyboard::BackSpace) {
                if(mUserInput.empty()) {
                    mServerIp.setString("\t\t\t server ip");
                    break;
                }
                mUserInput.pop_back();
                mServerIp.setString(mUserInput);
            }
            break;
        }
        case(sf::Event::EventType::KeyReleased): {
            if(event.key.code == sf::Keyboard::Enter) {
                if(mUserInput.empty()) {
                    mServerIp.setString("server ip");
                    break;
                }
                else {
                    // здесь инициализировать сервак
                    //char server_ip[256] = "127.0.0.1";
                    struct sockaddr_in server_addr;
                    int sockfd = initialize_client(PORT);
                    initialize_server(SERVER_PORT,mUserInput.c_str(),&server_addr);

                    setServerParams(sockfd, server_addr); // вот так записать данные в контекст

                    requestStackPop();
                    requestStackPush(States::Lobby);
                    break;
                }
            }
        }
        default:
            break;
    }
    return true;
}


