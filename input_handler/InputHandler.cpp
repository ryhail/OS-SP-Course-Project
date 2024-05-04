//
// Created by hvorostok on 01.05.2024.
//

#include <valarray>
#include <iostream>
#include "InputHandler.h"
#include "../entity_managment/Entity/EntityType.h"
#include "../player_managment/player/Player.h"
#include "../states/utility.h"

InputHandler::InputHandler() {
    // Set initial key bindings
    HotKey hotKeyForMoveLeft        = {sf::Keyboard::A};
    HotKey hotKeyForMoveDown        = {sf::Keyboard::S};
    HotKey hotKeyForMoveRight       = {sf::Keyboard::D};
    HotKey hotKeyForMoveUp          = {sf::Keyboard::W};
    HotKey hotKeyForMoveUpLeft      = {sf::Keyboard::A, sf::Keyboard::W};
    HotKey hotKeyForMoveDownLeft    = {sf::Keyboard::A, sf::Keyboard::S};
    HotKey hotKeyForMoveUpRight     = {sf::Keyboard::D, sf::Keyboard::W};
    HotKey hotKeyForMoveDownRight   = {sf::Keyboard::D, sf::Keyboard::S};

    mKeyBinding[hotKeyForMoveLeft]      = MoveLeft;
    mKeyBinding[hotKeyForMoveDown]      = MoveDown;
    mKeyBinding[hotKeyForMoveRight]     = MoveRight;
    mKeyBinding[hotKeyForMoveUp]        = MoveUp;
    mKeyBinding[hotKeyForMoveUpLeft]    = MoveUpLeft;
    mKeyBinding[hotKeyForMoveDownLeft]  = MoveDownLeft;
    mKeyBinding[hotKeyForMoveUpRight]   = MoveUpRight;
    mKeyBinding[hotKeyForMoveDownRight] = MoveDownRight;

    // Set initial action bindings
    initializeActions();

    // Assign all categories to player's aircraft
    for(auto& pair : mActionBinding)
    pair.second.category = EntityType::ACTIVE_PLAYER;
}

void InputHandler::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed)
    {
        //Check if pressed key appears in key binding, trigger command if so
        HotKey singleHotKey {event.key.code};
        auto found = mKeyBinding.find(singleHotKey);
        if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
            commands.push(mActionBinding[found->second]);
    }
}
/*
 * метод, обрабатывающий ввод в реальном времени
 * что даёт возможность передвижению
 * время сюда передаваться не должно, т.к.
 * в этом методе в очередь кладется лямбда-выражение
 * которое вызывается в SceneNode: void execCommand
 */
void InputHandler::handleRealtimeInput(CommandQueue& commands)
{
    // Traverse all assigned keys and check if they are pressed
//    for(const auto& pair: mKeyBinding)
//    {
//        bool completedHotKey = true;
//        for (sf::Keyboard::Key key : pair.first) {
//            if (!sf::Keyboard::isKeyPressed(key)) {
//                completedHotKey = false;
//            }
//        }
//        if(completedHotKey && isRealtimeAction(pair.second)) {
//            for(auto itr : pair.first) {
//                std::cout << toascii(itr) << " ";
//            }
//            commands.push(mActionBinding[pair.second]);
//        }
//    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        commands.push(mActionBinding[MoveUpLeft]);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        commands.push(mActionBinding[MoveUpRight]);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        commands.push(mActionBinding[MoveDownLeft]);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        commands.push(mActionBinding[MoveDownRight]);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        commands.push(mActionBinding[MoveUp]);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        commands.push(mActionBinding[MoveLeft]);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ) {
        commands.push(mActionBinding[MoveDown]);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ) {
        commands.push(mActionBinding[MoveRight]);
    }
}

void InputHandler::assignKey(Action action, const HotKey& hotKey)
{
    // Remove all keys that already map to action
    for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
    {
        if (itr->second == action)
            mKeyBinding.erase(itr++);
        else
            ++itr;
    }

    // Insert new binding
    mKeyBinding[hotKey] = action;
}

InputHandler::HotKey InputHandler::getAssignedKey(Action action) const
{
    for(const auto& pair: mKeyBinding)
    {
        if (pair.second == action)
            return pair.first;
    }

    return {};
}

void InputHandler::initializeActions()
{
    mActionBinding[MoveLeft].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(-1, 0), dt);
    });
    mActionBinding[MoveRight].action     = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(1, 0), dt);
    });
    mActionBinding[MoveUp].action        = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(0, 1), dt);
    });
    mActionBinding[MoveDown].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(0, 1), dt);
    });
    mActionBinding[MoveUpLeft].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(-1, 1), dt);
    });
    mActionBinding[MoveDownRight].action     = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(1, -1), dt);
    });
    mActionBinding[MoveUpRight].action        = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(1, 1), dt);
    });
    mActionBinding[MoveDownLeft].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(-1, -1), dt);
    });
}

bool InputHandler::isRealtimeAction(Action action)
{
    switch (action)
    {
        case MoveLeft:
        case MoveRight:
        case MoveDown:
        case MoveUp:
        case MoveUpRight:
        case MoveDownRight:
        case MoveDownLeft:
        case MoveUpLeft:
            return true;

        default:
            return false;
    }
}
