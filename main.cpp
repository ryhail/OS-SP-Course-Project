#include <SFML/Graphics.hpp>
#include "MainMenu.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "CubHead");
    MainMenu mainMenu;
    while (window.isOpen())
    {
        sf::Event event{};
        while(window.pollEvent(event))
        {
            switch(event.type) {
                case (sf::Event::Closed):
                    window.close();
                    break;
            }
        }
        window.setFramerateLimit(60);

        window.clear();

        mainMenu.draw(&window);

        window.display();
    }

    return 0;
}
