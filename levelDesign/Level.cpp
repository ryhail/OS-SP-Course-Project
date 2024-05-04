#include "Level.h"


Level::Level(sf::RenderWindow* window) :
    mWindow(window){
    currentMapTile = new MapTile(window);

}

void Level::update(sf::Time dt) {

}

void Level::draw() {
    currentMapTile->draw();
}

MapTile *Level::getCurrentMapTile() {
    return currentMapTile;
}




