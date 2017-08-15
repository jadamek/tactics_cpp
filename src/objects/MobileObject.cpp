#include "MobileObject.h"
#include "../settings.h"
#include <math.h>
#include <algorithm>

//----------------------------------------------------------------------------
// - Mobile Object Constructor
//----------------------------------------------------------------------------
// * ground : height map which the object's z-coodinate will be bound to
//----------------------------------------------------------------------------
MobileObject::MobileObject(const Map* ground) :
    IsometricObject(),
    AnimatedObject(),
    ground_(ground),
    speed_(1),
    arrival_(0)
{
    if(ground)
    {
        setPosition(sf::Vector3f(0, 0, std::max(0.f, ground->height(0, 0))));
    }
}

//----------------------------------------------------------------------------
// - Mobile Object Destructor
//----------------------------------------------------------------------------
MobileObject::~MobileObject()
{

}

//----------------------------------------------------------------------------
// - Move Linearly To Position
//----------------------------------------------------------------------------
// * destination : position the object will eventually arrive at
// Sets the object's target destination to a single location with a travel
// time dependant on the distance
//----------------------------------------------------------------------------
void MobileObject::moveTo(const sf::Vector3f& position)
{
    destination_ = position;
    arrival_ = computeArrival(position);
}

//----------------------------------------------------------------------------
// - Move Linearly To Position
//----------------------------------------------------------------------------
// * destination : position the object will eventually arrive at
// * duration : number of seconds until the object reaches the destination
// Sets the object's target destination to a single location with a fixed
// travel time
//----------------------------------------------------------------------------
void MobileObject::moveTo(const sf::Vector3f& position, float duration)
{
    if(duration > 0)
    {        
        destination_ = position;
        arrival_ = int(duration * getFPS() / speed_);
    }
}

//----------------------------------------------------------------------------
// - Is Object Moving?
//----------------------------------------------------------------------------
bool MobileObject::moving() const
{
    return arrival_ > 0;
}

//----------------------------------------------------------------------------
// - Stop Moving
//----------------------------------------------------------------------------
// Cancels current destination and motion
//----------------------------------------------------------------------------
void MobileObject::stopMoving()
{
    arrival_ = 0;
}

//----------------------------------------------------------------------------
// - Get Speed
//----------------------------------------------------------------------------
float MobileObject::getSpeed() const
{
    return speed_;
}

//----------------------------------------------------------------------------
// - Set Speed
//----------------------------------------------------------------------------
// * speed : relative speed, which multiplies the rate at which an object
//      moves (1 tile/s for constant motion)
//----------------------------------------------------------------------------
void MobileObject::setSpeed(float speed)
{
    if(speed > 0)
    {
        speed_ = speed;
    }
}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Propagates the object's motion along a height map, or 'ground'
//----------------------------------------------------------------------------
void MobileObject::step()
{
    if(arrival_ > 0)
    {
        float x = position().x + (destination_.x - position().x) / float(arrival_);
        float y = position().y + (destination_.y - position().y) / float(arrival_);
        float z = (ground_ ? ground_->height(x, y) : position().z);

        if(z < 0)
        {
            z = position().z;
        }

        setPosition(sf::Vector3f(x, y, z));
        arrival_--;
    }
}

//----------------------------------------------------------------------------
// - Compute Arrival (private)
//----------------------------------------------------------------------------
// * destination : final destination by which to compute total travel distance
//----------------------------------------------------------------------------
int MobileObject::computeArrival(const sf::Vector3f& position)
{
    double distance = sqrt(pow((position.x - position_.x), 2) + pow((position.y - position_.y), 2));
    return (int)(ceil(distance * getFPS() / speed_));
}