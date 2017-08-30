#include "IsometricObject.h"
#include "../map/IsometricNode.h"
#include "../map/IsometricBuffer.h"
#include "../settings.h"

//----------------------------------------------------------------------------
// - Isometric Object Contructor
//----------------------------------------------------------------------------
IsometricObject::IsometricObject() :
    sf::Drawable(),
    handler_(0)
{}
    
//----------------------------------------------------------------------------
// - Isometric Object Destructor
//----------------------------------------------------------------------------
IsometricObject::~IsometricObject()
{    
    if(handler_)
    {
        handler_->deactivate();
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
    bool changed = position != position_;

    position_ = position;

    if(changed && handler_ != 0)
    {
        // Alert handler of change
        handler_->alert(); 
    }
}

//----------------------------------------------------------------------------
// - Get Global Position from Isometric
//----------------------------------------------------------------------------
// Returns this object's 2-Dimensional pixel coordinate position by converting
// its 3-Dimensional isometric coordinate position
//----------------------------------------------------------------------------
sf::Vector2f IsometricObject::getGlobalPosition() const
{    
    return isoToGlobal(position());
}

//----------------------------------------------------------------------------
// - Get Global Position from Isometric
//----------------------------------------------------------------------------
// Returns the a 2-Dimensional pixel coordinate position by converting the
// 3-Dimensional isometric coordinate position
//----------------------------------------------------------------------------
sf::Vector2f IsometricObject::isoToGlobal(const sf::Vector3f& position)
{
    float x = 0.5 * MAP_SCALE.x * (position.x - position.y);
    float y = 0.5 * MAP_SCALE.y * (position.x + position.y) - MAP_SCALE.z * position.z;

    return sf::Vector2f(x, y);
}

//----------------------------------------------------------------------------
// - Join Buffer
//----------------------------------------------------------------------------
// * buffer : isometric container to join
// Overridable join method to allow composite joins
//----------------------------------------------------------------------------
void IsometricObject::join(IsometricBuffer* buffer) const
{
    buffer->insert(this);
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
// - Get Handler
//----------------------------------------------------------------------------
IsometricNode* IsometricObject::getHandler() const
{
    return handler_;
}