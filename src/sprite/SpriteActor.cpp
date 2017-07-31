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
    sf::FloatRect bounds;

    if(sprite_)
    {
        bounds = sprite_->getGlobalBounds();
        bounds.left += getPosition().x;
        bounds.top += getPosition().y;
    }
    
    return bounds;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void SpriteActor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(*sprite_, states);
}