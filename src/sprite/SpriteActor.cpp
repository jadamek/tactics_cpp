#include "SpriteActor.h"

//----------------------------------------------------------------------------
// - Actor Sprite Contructor
//----------------------------------------------------------------------------
SpriteActor::SpriteActor(const sf::Sprite* sprite) :
    sprite_(sprite)
{}

//----------------------------------------------------------------------------
// - Actor Sprite Destructor
//----------------------------------------------------------------------------
SpriteActor::~SpriteActor()
{
    delete sprite_;
}

//----------------------------------------------------------------------------
// - Set Sprite
//----------------------------------------------------------------------------
// * sprite : single sprite representing this actor
//----------------------------------------------------------------------------
void SpriteActor::setSprite(const sf::Sprite* sprite)
{
    sprite_ = sprite;
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this actor's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect SpriteActor::getGlobalBounds() const
{
    if(sprite_)
    {
        return getTransform().transformRect(sprite_->getGlobalBounds());
    }
    
    return sf::FloatRect();
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void SpriteActor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(*sprite_, states);
}