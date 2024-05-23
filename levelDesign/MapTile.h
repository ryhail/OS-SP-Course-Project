#ifndef SFML_MAPTILE_H
#define SFML_MAPTILE_H

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "../resources/ResourceHolder.h"

typedef ResourceHolder<Textures::ID, sf::Texture> TextureHolder;

class MapTile {
    std::vector<Tile>               mTiles;
    TextureHolder                   mTextures;
    sf::RenderWindow*               mWindow;
    sf::Vector2f                    mSpawnPoint;
public:
    MapTile(sf::RenderWindow* window);
    MapTile(sf::RenderWindow* windowm, char schema[11][21]);
    void draw();
    sf::Vector2f                    getSpawnPoint();
    Tile::Type     getCurrentTileType(sf::Vector2f);
};


#endif //SFML_MAPTILE_H
