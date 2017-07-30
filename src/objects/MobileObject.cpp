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
// Sets the object's target destination to a single location
//----------------------------------------------------------------------------
void MobileObject::moveTo(const sf::Vector3f& position)
{
    destination_.clear();
    destination_.push_back(position);
    arrival_ = computeArrival(position);
}

//----------------------------------------------------------------------------
// - Move Along a Path
//----------------------------------------------------------------------------
// * path : a list of consecutive destinations
// Sets the object's destination as a path, or list of consecutive locations
//----------------------------------------------------------------------------
void MobileObject::moveAlong(const std::list<sf::Vector3f>& path)
{
    destination_ = path;
    arrival_ = (path.empty() ? 0 : computeArrival(path.front()));
}

//----------------------------------------------------------------------------
// - Is Object Moving?
//----------------------------------------------------------------------------
bool MobileObject::moving() const
{
    destination_.size() > 0;
}

//----------------------------------------------------------------------------
// - Stop Moving
//----------------------------------------------------------------------------
// Cancels current destination and motion
//----------------------------------------------------------------------------
void MobileObject::stopMoving()
{
    destination_.clear();
    arrival_ = 0;
}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Propagates the object's motion along a height map, or 'ground'
//----------------------------------------------------------------------------
void MobileObject::step()
{
    if(destination_.size())
    {
        if(arrival_ > 0)
        {
            float x = position().x + (destination_.front().x - position().x) / float(arrival_);
            float y = position().y + (destination_.front().y - position().y) / float(arrival_);
            float z = (ground_ ? ground_->height(x, y) : position().z);

            if(z < 0)
            {
                z = position().z;
            }

            setPosition(sf::Vector3f(x, y, z));
            arrival_--;
        }

        if(arrival_ == 0)
        {
            destination_.pop_front();

            if(destination_.size() > 0)
            {
                arrival_ = computeArrival(destination_.front());
            }
        }
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
    return (int)(ceil(distance * getFPS()));
}