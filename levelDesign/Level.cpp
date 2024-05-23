#include "Level.h"


Level::Level(sf::RenderWindow* window) :
    mWindow(window){
    char schema[11][21] = {"00000000000000000000",
                           "01111111111111111110",
                           "01111111111111111110",
                           "01111111122111111110",
                           "01111121112111111110",
                           "01111111111111111110",
                           "01111113331111111110",
                           "01111211111111411110",
                           "01111111111111111110",
                           "01111111311111411110",
                           "00000000000000000000"};
    currentMapTile = new MapTile(window, schema);

}

void Level::update(sf::Time dt) {

}

void Level::draw() {
    currentMapTile->draw();
}

MapTile *Level::getCurrentMapTile() {
    return currentMapTile;
}




