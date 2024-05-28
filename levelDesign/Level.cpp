#include <sys/socket.h>
#include "Level.h"


Level::Level(sf::RenderWindow *window) :
    mWindow(window){
    char schema[11][21];
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 20; j++) {
            if(i == 0 || i == 10 || j == 0 || j == 19) {
                schema[i][j] = '0';
                continue;
            }
            schema[i][j] = '1';
        }
    }

    //srand(seed);
    for(int i = 0; i < 40; i++) {
        schema[rand() % 9 + 1][rand() % 18 + 1] = (char)('1' + rand() % 4);
    }
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




