#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "TestServer.h"
#include "UserInterface.h"
#include "ConnectToServer.h"
#include "server/client.c"
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "CubHead");
    MainMenu mainMenu;
    TestServer testServer;
    UserInterface userInterface;
    ConnectToServer connectToServer;
    while (window.isOpen())
    {
        sf::Event event{};
        while(window.pollEvent(event))
        {
//            if(mainMenu.isActive())
//                mainMenu.update(&event);
            connectToServer.update(&event);

            if(event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::W:
                        //отправка на сервер движения вверх
                        break;
                    case sf::Keyboard::S:
                        //отправка на сервер движения вниз
                        break;
                    case sf::Keyboard::A:
                        //влево
                        break;
                    case sf::Keyboard::D:
                        //вправо
                        break;
                    default:
                        break;
                }
                //Принимаем с сервера новую позицию первого квадрата
                //testServer.updatePositionFirst(sf::Vector2i(x, y))
                //Принимаем с сервера новую позицию второго квадрата
                //testServer.updatePositionSecond(sf::Vector2i(x, y))
            }
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }
        userInterface.update(5);
        window.setFramerateLimit(60);
        window.clear();
//        if(mainMenu.isActive())
//            mainMenu.draw(&window);
        if(testServer.isActive())
            testServer.draw(&window);
        if(userInterface.isActive())
            userInterface.draw(&window);
        connectToServer.draw(&window);
        window.display();
    }

    return 0;
}
