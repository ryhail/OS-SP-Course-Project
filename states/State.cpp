#include "State.h"
#include "StateStack.h"

State::Context::Context(sf::RenderWindow& window,
        ResourceHolder<Textures::ID, sf::Texture>* textures,
        ResourceHolder<Fonts::ID, sf::Font>* fonts)
        : window(&window),
        textures(textures),
        fonts(fonts)
{
    textures->load(Textures::MainMenu, "resources/Textures/menubg.png");
    textures->load(Textures::Lobby, "resources/Textures/lobbybg.png");
    textures->load(Textures::Player1, "resources/Textures/player1.png");
    textures->load(Textures::Player2, "resources/Textures/player2.png");
    textures->load(Textures::Bullet, "resources/Textures/bullet.png");
    textures->load(Textures::Heart, "resources/Textures/heart.png");
    textures->load(Textures::PickUpIncreaseBullet, "resources/Textures/bullet.png");
    textures->load(Textures::PickUpRefillHealth, "resources/Textures/heart.png");
    player1 = new Player(textures, Textures::Player1);
    player2 = new Player(textures, Textures::Player2);
    sockfd = new int;
    server_adr = new sockaddr_in;
}

State::State(StateStack& stack, Context context)
        : mStack(&stack)
        , mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
    mStack->pushState(stateID);
}

void State::requestStackPop()
{
    mStack->popState();
}

void State::requestStateClear()
{
    mStack->clearStates();
}

State::Context State::getContext() const
{
    return mContext;
}

void State::setServerParams(int sockfd, sockaddr_in server_adr) {
    *mContext.sockfd = sockfd;
    *mContext.server_adr = server_adr;
}


