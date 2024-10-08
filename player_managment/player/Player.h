#ifndef COURSE_PLAYER_H
#define COURSE_PLAYER_H


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../entity_managment/Bullet/Bullet.h"
#include "../../entity_managment/Entity/EntityType.h"
#include "../../command/CommandQueue.h"
#include "../../levelDesign/MapTile.h"
#include "../Heart.h"

#define PLAYER_INIT_SPEED 100
#define PLAYER_INIT_HITPOINTS 5
#define PLAYER_FIRING_INTERVAL sf::seconds(2.0f)
#define PLAYER_HILING_INTERVAL sf::seconds(5.0f)
#define PICKTIME sf::seconds(3)

enum Animation {
    Idle,
    Left,
    Right,
    Up,
    Down
};

class Player : public Entity{
    TextureHolder*  mTextures;
    sf::Sprite      playerSprite;
    sf::Sprite      heart;
    MapTile*        currentMapTile;
    sf::Time        animationDeltaTime;
    sf::Time        surfaceDeltaTime;
    int             animationFrame;
    Animation       currentAnimation;
    bullet_t *      lastBulletCreated;

public:
    bool            spike;
    Player(TextureHolder* textures, Textures::ID playerType);
    void    takeDamage(int dmg);
    void    heal();
    void    updateFacing(float x, float y);
    void    setFiringShift(float shiftX, float shiftY);
    void    setActive(bool value);
    void    move(sf::Vector2i direction, sf::Time time);
    void    fire();
    bool    isActive() const;
    void    draw(sf::RenderWindow* window);
    bool    isForRemove() override;
    void    setCurentMapTile(MapTile*);
    void    animate(Animation AnimType, sf::Time dt);
    void    setPosition(float x, float y);
    void    setPosition(sf::Vector2f pos);
    void    takeBullets(int bullets);
    bool    isDead();
    void drawHearts(sf::RenderWindow *window);
    void updateSurface(sf::Time dt);
    Animation getCurrentAnimation();
    void setCurrentAnimation(Animation animType);
    bullet_t getLastBullet();

    sf::Vector2f getCoordinates() override;

    sf::FloatRect   getBoundingRect() const override;
    sf::Rect<float> getPlayerSpriteSize() const;
    void setHitPoints(int hp);
    int getHitPoints() override;

private:
    void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const override;
    void checkBulletLaunch(CommandQueue& commandQueue, sf::Time dt);
    void checkPickUpLaunch(CommandQueue& commandQueue, sf::Time dt);
    bool firingAvailable() const;
    void createBullet(SceneNode &node, TextureHolder &textures);
    void decrementBulletCount();
    void createPickUp(SceneNode &node, TextureHolder &textures);

    void updateCurrent(sf::Time dt, CommandQueue &queue) override;

public:
    int     getSpeed() const;
    int     getHitPoints() const;

private:
    sf::Time    fireCountdown;
    sf::Time    pickUpCountdown;
    bool        isFiring;
    Command     fireCommand;
    Command     pickUpCommand;
    int         bulletCount;

    int         hitPoints;
    sf::Time    healingTime;

    int         speed;
    bool        active;

    sf::Vector2<float> facing;
    sf::Vector2<float> firingShift;

    EntityType::Type getCategory() const override;
};


#endif //COURSE_PLAYER_H