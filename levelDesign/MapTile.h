#ifndef SFML_MAPTILE_H
#define SFML_MAPTILE_H

#include <SFML/Graphics.hpp>
#include "../entity_managment/Bullet/Bullet.h"
#include "Tile.h"

class MapTile {
    std::vector<Tile>               mTiles;
    TextureHolder                   mTextures;
    sf::RenderWindow*               mWindow;
    sf::Vector2f                    mSpawnPoint;
public:
    MapTile(sf::RenderWindow* window);
    void draw();
    sf::Vector2f                    getSpawnPoint();
};


#endif //SFML_MAPTILE_H
