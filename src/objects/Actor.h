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
// Methods
public:
    Actor(const Sprite* sprite = 0, const Map* ground = 0);
    virtual ~Actor();

    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect   getGlobalBounds() const;
    void                    setSprite(const Sprite* sprite);
    const Sprite*           getSprite() const;

protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    const Sprite*           sprite_;
};

#endif