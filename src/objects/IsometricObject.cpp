#include "IsometricObject.h"
#include "../map/IsometricBuffer.h"

//----------------------------------------------------------------------------
// - Isometric Object Contructor
//----------------------------------------------------------------------------
IsometricObject::IsometricObject() :
    sf::Drawable(),
    handler_(0){}

//----------------------------------------------------------------------------
// - Isometric Object Destructor
//----------------------------------------------------------------------------
IsometricObject::~IsometricObject()
{
    if(handler_)
    {
        delete handler_;
    }
}

//----------------------------------------------------------------------------
// - Set Position
//----------------------------------------------------------------------------
// Returns the 3-dimensional position of the object in isometric space
//----------------------------------------------------------------------------
const sf::Vector3f& IsometricObject::position() const
{
    return position_;
}

//----------------------------------------------------------------------------
// - Get Height
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the object
//----------------------------------------------------------------------------
float IsometricObject::getHeight(const sf::Vector2f& position) const
{
    return 1.0;
}

//----------------------------------------------------------------------------
// - Set Position
//----------------------------------------------------------------------------
// * position : new 3-D position of the object
//----------------------------------------------------------------------------
void IsometricObject::setPosition(const sf::Vector3f& position)
{
    if(position != position_)
    {
       // Alert handler of change
        handler_->update(); 
    }

    position_ = position;
}

//----------------------------------------------------------------------------
// - Set Handler
//----------------------------------------------------------------------------
// * handler : new IsometricNode to handle the object during sorting
//----------------------------------------------------------------------------
void IsometricObject::setHandler(IsometricNode* node)
{
    handler_ = node;
}

//----------------------------------------------------------------------------
// - Remove Handler
//----------------------------------------------------------------------------
// Sets the IsometricNode handler to NULL
//----------------------------------------------------------------------------
void IsometricObject::removeHandler()
{
    handler_ = 0;
}