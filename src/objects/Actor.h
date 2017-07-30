#ifndef TACTICS_ACTOR_H
#define TACTICS_ACTOR_H

#include "MobileObject.h"
#include "../sprite/Sprite.h"

//================================================================================
// ** Actor
//================================================================================
// Represents an isometric object as a mobile sprite
//================================================================================
class Actor : public MobileObject
{
public:
    Actor(const Sprite* sprite = 0, const Map* ground = 0);
    ~Actor();

    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect   getGlobalBounds() const;
    void                    setSprite(const Sprite* sprite);

protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

    const Sprite*           sprite_;
};

#endif