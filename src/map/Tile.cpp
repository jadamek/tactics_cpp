#include "Tile.h"
#include <algorithm>

//----------------------------------------------------------------------------
// - Tile Constructor
//----------------------------------------------------------------------------
Tile::Tile(const Sprite* sprite, float height) :
    MapObject(height),
    sprite_(sprite),
    occupant_(0)
{}

//----------------------------------------------------------------------------
// - Tile Destructor
//----------------------------------------------------------------------------
Tile::~Tile()
{
    if(sprite_){
        delete sprite_;        
    }
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this tile's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect Tile::getGlobalBounds() const
{
    if(sprite_)
    {
        return sprite_->getGlobalBounds();        
    }
    else
    {
        return sf::FloatRect();
    }
}

//----------------------------------------------------------------------------
// Get Occupant Tile
//----------------------------------------------------------------------------
const MapObject* Tile::getOccupant() const
{
    return occupant_;
}

//----------------------------------------------------------------------------
// Get Occupant Tile (Mutable)
//----------------------------------------------------------------------------
MapObject* Tile::getOccupant()
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
void Tile::setOccupant(MapObject* occupant)
{
    occupant_ = occupant;
}

//----------------------------------------------------------------------------
// - Rise (Override)
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
// - Lower (Override)
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
    if(sprite_)
    {
        target.draw(*sprite_, states);        
    }
}