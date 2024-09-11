//
// Created by hvorostok on 01.05.2024.
//

#ifndef SFML_INPUTHANDLER_H
#define SFML_INPUTHANDLER_H


#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <map>
#include "../command/CommandQueue.h"

#define DEFAULT_KEY sf::KeyBoard::Z

class InputHandler {
public:

    /*
     * HotKey введен непосредственно для комбинаций клавиш
     * например, поход по диагонали требует ввода сразу двух клавиш
     */
    typedef std::vector<sf::Keyboard::Key> HotKey;

    enum Action
    {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        MoveUpRight,
        MoveDownRight,
        MoveUpLeft,
        MoveDownLeft,
        FireLeft,
        FireRight,
        FireUp,
        FireDown,
        FireUpRight,
        FireDownRight,
        FireUpLeft,
        FireDownLeft,
        ActionCount
    };
    InputHandler();

    void					                handleEvent(const sf::Event& event, CommandQueue& commands);
    void					                handleRealtimeInput(CommandQueue& commands);

    void					                assignKey(Action action, const sf::Keyboard::Key& hotKey);
    [[nodiscard]] sf::Keyboard::Key 		getAssignedKey(Action action) const;

private:
    void					initializeActions();
    static bool				isRealtimeAction(Action action);


private:
    std::map<sf::Keyboard::Key, Action>		mKeyBinding;
    std::map<Action, Command>	            mActionBinding;

};


#endif //SFML_INPUTHANDLER_H
