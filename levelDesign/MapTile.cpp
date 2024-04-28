#include "MapTile.h"


MapTile::MapTile(sf::RenderWindow* window) :
        mWindow(window){
    mTextures.load(Textures::Tile, "resources/Textures/tile.png");
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 20; j++) {

            sf::Sprite tile;
            if( i == 0 || j == 0 || i == 10 || j == 19) {
                continue;
            }
            tile.setTexture(mTextures.getResource(Textures::Tile));
            tile.setPosition(  j * 64,  i * 64);
            mTiles.push_back(tile);
        }
    }
}

void MapTile::draw() {
    for(auto& tile: mTiles)
        mWindow->draw(tile);
}
