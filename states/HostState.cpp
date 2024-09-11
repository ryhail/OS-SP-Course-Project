#include "HostState.h"
#include "../server/client.h"

HostState::HostState(StateStack &stack, State::Context context) : State(stack, context) {
    struct sockaddr_in server_addr;
    int sockfd = initialize_client(PORT);
    initialize_server(SERVER_PORT,"127.0.0.1",&server_addr);
    setServerParams(sockfd, server_addr);
}

void HostState::draw() {

}

bool HostState::update(sf::Time dt) {
    requestStackPop();
    requestStackPush(States::Lobby);
    return true;
}

bool HostState::handleEvent(const sf::Event &event) {
    return true;
}
