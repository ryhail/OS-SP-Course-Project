#ifndef COURSE_PLAYER_H
#define COURSE_PLAYER_H


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../entity_managment/Bullet/Bullet.h"
#include "../../entity_managment/Entity/EntityType.h"
#include "../../command/CommandQueue.h"
#include "../../levelDesign/MapTile.h"

#define PLAYER_INIT_SPEED 100
#define PLAYER_INIT_HITPOINTS 5
#define PLAYER_FIRING_INTERVAL sf::seconds(2.0f)
#define PLAYER_HILING_INTERVAL sf::seconds(5.0f)

enum Animation {
    Idle,
    Left,
    Right,
    Up,
    Down
};

class Player : public Entity{

    sf::Sprite      playerSprite;
    MapTile*        currentMapTile;
    sf::Time        animationDeltaTime;
    int             animationFrame;

public:
    Player(TextureHolder* textures, Textures::ID playerType);
    void    takeDamage(int dmg);
    void    heal(int heal);
    void    updateFacing(float x, float y);
    void    setFiringShift(float shiftX, float shiftY);
    void    setSpritePosition(sf::Vector2f _coords);
    void    setActive(bool value);
    void    move(sf::Vector2i direction, sf::Time time);
    void    fire();
    bool    isActive() const;
    void    draw(sf::RenderWindow* window);
    bool    isForRemove(sf::RenderWindow &window) override;
    void    setCurentMapTile(MapTile*);
    void    animate(Animation AnimType, sf::Time dt);

    sf::Rect<float> getPlayerSpriteSize() const;
private:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;

    void checkBulletLaunch(CommandQueue& commandQueue, sf::Time dt);
    bool firingAvailable() const;
    void createBullet(SceneNode &node, TextureHolder &textures);
    void decrementBulletCount();

    void updateCurrent(sf::Time dt, CommandQueue &queue) override;

public:
    int     getSpeed() const;

private:
    sf::Time    fireCountdown;
    bool        isFiring;
    Command     fireCommand;
    int         bulletCount;
    sf::Time    healingTime;
    int         hitPoints;
    int         speed;
    bool        active;

    sf::Vector2<float> facing;
    sf::Vector2<float> firingShift;

    EntityType::Type getCategory() const override;
};


#endif //COURSE_PLAYER_H
