#ifndef SFML_MAPTILE_H
#define SFML_MAPTILE_H

#include <SFML/Graphics.hpp>
#include "../entity_managment/Bullet/Bullet.h"

class MapTile {
    std::vector<sf::Sprite> mTiles;
    TextureHolder mTextures;
    sf::RenderWindow* mWindow;
public:
    MapTile(sf::RenderWindow* window);
    void draw();
};


#endif //SFML_MAPTILE_H
