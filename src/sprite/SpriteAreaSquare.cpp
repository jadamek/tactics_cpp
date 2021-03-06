#include "SpriteAreaSquare.h"

//----------------------------------------------------------------------------
// - Sprite Area Constructor
//----------------------------------------------------------------------------
// * texture : bitmap of a single area square
// * color : coloring of the affected area, e.g blue for a move square
//----------------------------------------------------------------------------
SpriteAreaSquare::SpriteAreaSquare(const sf::Texture& texture, const sf::Color& color) :
    sprite_(texture)
{
    sprite_.setColor(color);
    sprite_.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

//----------------------------------------------------------------------------
// - Sprite Area Destructor
//----------------------------------------------------------------------------
SpriteAreaSquare::~SpriteAreaSquare()
{}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this actor's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect SpriteAreaSquare::getGlobalBounds() const
{
    return sprite_.getGlobalBounds();
}

//----------------------------------------------------------------------------
// - Get Height (Override)
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the object
//----------------------------------------------------------------------------
float SpriteAreaSquare::getHeight(const sf::Vector2f& position) const
{
    return 0;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void SpriteAreaSquare::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite_, states);
}