#ifndef SFML_LEVEL_H
#define SFML_LEVEL_H


#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics.hpp>
#include "../entity_managment/SceneNode.h"
#include "../entity_managment/Bullet/Bullet.h"
#include "../player_managment/player/Player.h"
#include "MapTile.h"

class Level : private sf::NonCopyable{
public:
    Level(sf::RenderWindow *window, int seed);
    void								update(sf::Time dt);
    void								draw();
    MapTile*                            getCurrentMapTile();
private:
    std::vector<MapTile*>               mapTiles;
    MapTile*                            currentMapTile;

private:
    sf::RenderWindow*					mWindow;
    TextureHolder						mTextures;
};



#endif //SFML_LEVEL_H
