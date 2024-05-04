#ifndef SFML_TILE_H
#define SFML_TILE_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Tile : public sf::Sprite {
public:
    enum Type {
        Border,
        field
    };
private:
    Type       mType;
public:

    Tile();
    void    setType(Type type);
    Type    getType();
};


#endif //SFML_TILE_H
