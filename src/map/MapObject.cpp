#include "MapObject.h"

//----------------------------------------------------------------------------
// - MapObject Constructor
//----------------------------------------------------------------------------
MapObject::MapObject(float height) :
    IsometricObject(),
    height_(std::max(0.f, height)){}

//----------------------------------------------------------------------------
// - MapObject Destructor
//----------------------------------------------------------------------------
MapObject::~MapObject()
{}

//----------------------------------------------------------------------------
// - Get Height
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the object
//----------------------------------------------------------------------------
float MapObject::getHeight(const sf::Vector2f& position) const
{
    return height_;
}

//----------------------------------------------------------------------------
// - Set Height
//----------------------------------------------------------------------------
// * height : new height of the object
// Raises or lowers the object appropriately
//----------------------------------------------------------------------------
void MapObject::setHeight(float height)
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
// * z : relative height to raise this MapObject by.
// Raises a MapObject increasing its z-position
//----------------------------------------------------------------------------
void MapObject::rise(float z)
{    
    setPosition(sf::Vector3f(position_.x, position_.y, position_.z += z));
}


//----------------------------------------------------------------------------
// - Lower 
//----------------------------------------------------------------------------
// * z : relative height to lower this object by.
// Lowers a MapObject decreasing its z-position
//----------------------------------------------------------------------------
void MapObject::lower(float z)
{
    setPosition(sf::Vector3f(position_.x, position_.y, position_.z -= z));
}