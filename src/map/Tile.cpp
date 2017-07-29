#include "Tile.h"
#include <algorithm>

//----------------------------------------------------------------------------
// - Tile Constructor
//----------------------------------------------------------------------------
Tile::Tile(const Sprite* sprite, float height) :
    IsometricObject(),
    sprite_(sprite),
    occupant_(0),
    height_(std::max(0.f, height)){}

//----------------------------------------------------------------------------
// - Get Height
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the object
//----------------------------------------------------------------------------
float Tile::getHeight(const sf::Vector2f& position) const
{
    return height_;
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this tile's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect Tile::getGlobalBounds() const
{
    return sprite_->getGlobalBounds();
}

//----------------------------------------------------------------------------
// Get Occupant Tile
//----------------------------------------------------------------------------
const Tile* Tile::getOccupant() const
{
    return occupant_;
}

//----------------------------------------------------------------------------
// Get Occupant Tile (Mutable)
//----------------------------------------------------------------------------
Tile* Tile::getOccupant()
{
    return occupant_;
}

//----------------------------------------------------------------------------
// - Set Sprite
//----------------------------------------------------------------------------
// * sprite : new sprite image this tile displays
//----------------------------------------------------------------------------
void Tile::setSprite(const Sprite* sprite)
{
    sprite_ = sprite;
}

//----------------------------------------------------------------------------
// - Set Occupant
//----------------------------------------------------------------------------
// * occupant : set the map object lying on top of this one
//----------------------------------------------------------------------------
void Tile::setOccupant(Tile* occupant)
{
    occupant_ = occupant;
}

//----------------------------------------------------------------------------
// - Set Height
//----------------------------------------------------------------------------
// * height : new height of the tile
// Raises or lowers the object appropriately
//----------------------------------------------------------------------------
void Tile::setHeight(float height)
{
    float heightChange = height - height_;

    if(heightChange > 0)
    {
        rise(heightChange);
    }
    else
    {
        lower(-1 * heightChange);
    }
}

//----------------------------------------------------------------------------
// - Rise
//----------------------------------------------------------------------------
// * z : relative height to raise this tile by.
// Raises a tile increasing its z-position and that of any object above it.
//----------------------------------------------------------------------------
void Tile::rise(float z)
{    
    setPosition(sf::Vector3f(position_.x, position_.y, position_.z += z));
    if(occupant_)
    {
        occupant_->rise(z);
    }
}


//----------------------------------------------------------------------------
// - Lower 
//----------------------------------------------------------------------------
// * z : relative height to lower this tile by.
// Lowers a tile decreasing its z-position and that of any object above it.
//----------------------------------------------------------------------------
void Tile::lower(float z)
{
    setPosition(sf::Vector3f(position_.x, position_.y, position_.z -= z));
    if(occupant_)
    {
        occupant_->lower(z);
    }
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*sprite_, states);
}