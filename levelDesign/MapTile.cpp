#include <cmath>
#include "MapTile.h"


MapTile::MapTile(sf::RenderWindow* window) :
        mWindow(window){
    mTextures.load(Textures::Tile, "resources/Textures/tile.png");
    mTextures.load(Textures::Border, "resources/Textures/border.png");
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 20; j++) {

            Tile tile;
            if( i == 0 || j == 0 || i == 10 || j == 19) {
                tile.setType(Tile::Type::Border);
                tile.setTexture(mTextures.getResource(Textures::Border));
                tile.setPosition(  j * 64,  i * 64);
                mTiles.push_back(tile);
                continue;
            }
            tile.setTexture(mTextures.getResource(Textures::Tile));
            tile.setType(Tile::Type::field);
            tile.setPosition(  j * 64,  i * 64);
            mTiles.push_back(tile);
        }
    }
    for(auto& tile: mTiles) {
        if(tile.getType() == Tile::Type::field) {
            mSpawnPoint = tile.getPosition() + (sf::Vector2f(64,64) / 2.f);
            break;
        }
    }
}

void MapTile::draw() {
    for(auto& tile: mTiles)
        mWindow->draw(tile);
}

sf::Vector2f MapTile::getSpawnPoint() {
    return mSpawnPoint;
}

Tile::Type MapTile::getCurrentTileType(sf::Vector2f _coords){
    sf::Vector2i tile_coodinates;
    tile_coodinates.x =  floor(_coords.x/64);
    tile_coodinates.y = floor(_coords.y/64);
    return (mTiles.begin() + tile_coodinates.x + (tile_coodinates.y*20))->getType();
}


