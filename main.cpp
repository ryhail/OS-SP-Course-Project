#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "CubHead");
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

        window.display();
    }

    return 0;
}
