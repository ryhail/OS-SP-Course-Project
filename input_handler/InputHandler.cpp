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
    sf::Keyboard::Key hotKeyForMoveLeft         = sf::Keyboard::A;
    sf::Keyboard::Key hotKeyForMoveDown         = sf::Keyboard::S;
    sf::Keyboard::Key hotKeyForMoveRight        = sf::Keyboard::D;
    sf::Keyboard::Key hotKeyForMoveUp           = sf::Keyboard::W;

    sf::Keyboard::Key hotKeyForFireUp           = sf::Keyboard::Up;
    sf::Keyboard::Key hotKeyForFireLeft         = sf::Keyboard::Left;
    sf::Keyboard::Key hotKeyForFireRight        = sf::Keyboard::Right;
    sf::Keyboard::Key hotKeyForFireDown         = sf::Keyboard::Down;

    mKeyBinding[hotKeyForMoveLeft]      = MoveLeft;
    mKeyBinding[hotKeyForMoveDown]      = MoveDown;
    mKeyBinding[hotKeyForMoveRight]     = MoveRight;
    mKeyBinding[hotKeyForMoveUp]        = MoveUp;

    mKeyBinding[hotKeyForFireUp]        = FireUp;
    mKeyBinding[hotKeyForFireLeft]      = FireLeft;
    mKeyBinding[hotKeyForFireRight]     = FireRight;
    mKeyBinding[hotKeyForFireDown]      = FireDown;

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
        auto found = mKeyBinding.find(event.key.code);
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            commands.push(mActionBinding[MoveUpRight]);
            return;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            commands.push(mActionBinding[MoveUpLeft]);
            return;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            commands.push(mActionBinding[MoveDownRight]);
            return;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            commands.push(mActionBinding[MoveDownLeft]);
            return;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            commands.push(mActionBinding[FireUpRight]);
            return;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            commands.push(mActionBinding[FireUpLeft]);
            return;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            commands.push(mActionBinding[FireDownRight]);
            return;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            commands.push(mActionBinding[FireDownLeft]);
            return;
        }
    }
    //Traverse all assigned keys and check if they are pressed
    for(const auto& pair: mKeyBinding)
    {
        // If key is pressed, lookup action and trigger corresponding command
        if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
            commands.push(mActionBinding[pair.second]);
    }
}

void InputHandler::assignKey(Action action, const sf::Keyboard::Key& hotKey)
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

sf::Keyboard::Key InputHandler::getAssignedKey(Action action) const
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
        player.move(sf::Vector2i(0, -1), dt);
    });
    mActionBinding[MoveDown].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(0, 1), dt);
    });
    mActionBinding[MoveUpLeft].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(-1, -1), dt);
    });
    mActionBinding[MoveDownRight].action     = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(1, 1), dt);
    });
    mActionBinding[MoveUpRight].action        = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(1, -1), dt);
    });
    mActionBinding[MoveDownLeft].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.move(sf::Vector2i(-1, 1), dt);
    });
    mActionBinding[FireUp].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.updateFacing(0, 1);
        player.fire();
    });
    mActionBinding[FireUpRight].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.updateFacing(1, 1);
        player.fire();
    });
    mActionBinding[FireUpLeft].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.updateFacing(-1, 1);
        player.fire();
    });
    mActionBinding[FireRight].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.updateFacing(1, 0);
        player.fire();
    });
    mActionBinding[FireLeft].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.updateFacing(-1, 0);
        player.fire();
    });
    mActionBinding[FireDown].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.updateFacing(0, -1);
        player.fire();
    });
    mActionBinding[FireDownRight].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.updateFacing(1, -1);
        player.fire();
    });
    mActionBinding[FireDownLeft].action      = derivedAction<Player>([](Player& player, sf::Time dt) {
        player.updateFacing(-1, -1);
        player.fire();
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
        case FireUp:
        case FireDown:
        case FireDownLeft:
        case FireDownRight:
        case FireLeft:
        case FireRight:
        case FireUpLeft:
        case FireUpRight:
            return true;

        default:
            return false;
    }
}
