#include "Actor.h"

//----------------------------------------------------------------------------
// - Actor Constructor
//----------------------------------------------------------------------------
// * sprite : representative sprite for this actor
//----------------------------------------------------------------------------
Actor::Actor(const Sprite* sprite, const Map* ground) :
    MobileObject(ground),
    sprite_(sprite)
{}

//----------------------------------------------------------------------------
// - Actor Destructor
//----------------------------------------------------------------------------
Actor::~Actor()
{
    delete sprite_;
}

//----------------------------------------------------------------------------
// - Get Height (Override)
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the actor
//----------------------------------------------------------------------------
float Actor::getHeight(const sf::Vector2f& position) const
{
    return 3;
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this actor's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect Actor::getGlobalBounds() const
{
    if(sprite_)
    {
        return sprite_->getGlobalBounds();
    }
    else{
        return sf::FloatRect(position_.x, position_.y, 0, 0);
    }
}

//----------------------------------------------------------------------------
// - Set Sprite
//----------------------------------------------------------------------------
// * sprite : representative sprite for this actor
//----------------------------------------------------------------------------
void Actor::setSprite(const Sprite* sprite)
{
    sprite_ = sprite;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(sprite_)
    {
        target.draw(*sprite_, states);
    }
}
