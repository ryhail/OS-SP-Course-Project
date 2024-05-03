#ifndef SFML_UTILITY_H
#define SFML_UTILITY_H

namespace sf
{
    class Sprite;
    class Text;
    class RectangleShape;
}

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(sf::RectangleShape& rect);

int sign(int shislo);

#endif //SFML_UTILITY_H
