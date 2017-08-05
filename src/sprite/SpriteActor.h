#ifndef TACTICS_SPRITE_ACTOR_H
#define TACTICS_SPRITE_ACTOR_H

#include "Sprite.h"

//================================================================================
// ** Sprite Actor
//================================================================================
// A sprite of an active (mobile) character in the world -> subject to change as
// this is merely as testing base for now.
//================================================================================
class SpriteActor : public Sprite
{
// Methods
public:
    SpriteActor(const sf::Sprite* sprite = 0);
    virtual ~SpriteActor();

    void                    setSprite(const sf::Sprite* sprite);
    virtual sf::FloatRect   getGlobalBounds() const;

protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    const sf::Sprite*       sprite_;
};

#endif