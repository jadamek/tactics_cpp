#include "MobileObject.h"
#include "../settings.h"
#include <math.h>

//----------------------------------------------------------------------------
// - Mobile Object Constructor
//----------------------------------------------------------------------------
// * ground : height map which the object's z-coodinate will be bound to
//----------------------------------------------------------------------------
MobileObject::MobileObject(const Map* ground) :
    speed_(1),
    frozen_(false),
    ground_(ground),
    clock_(0),
    arrival_(0)
{
    if(ground)
    {
        setPosition(sf::Vector3f(0, 0, ground->height(0, 0)));
    }
}

//----------------------------------------------------------------------------
// - Mobile Object Destructor
//----------------------------------------------------------------------------
MobileObject::~MobileObject()
{

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
// * speed : relative multiplier of time that passes for this object
//----------------------------------------------------------------------------
void MobileObject::setSpeed(float speed)
{
    // A speed of zero or less is undefined
    if(speed > 0)
    {
        speed_ = speed;
    }
}

//----------------------------------------------------------------------------
// - Is Object Frozen?
//----------------------------------------------------------------------------
bool MobileObject::frozen() const
{
    return frozen_;
}

//----------------------------------------------------------------------------
// - Set Frozen Status
//----------------------------------------------------------------------------
// * frozen : sets a flag indicating time is frozen for this object
//----------------------------------------------------------------------------
void MobileObject::setFrozen(bool frozen)
{
    frozen_ = frozen;
}

//----------------------------------------------------------------------------
// - Set Frozen to True
//----------------------------------------------------------------------------
void MobileObject::freeze()
{
    frozen_ = true;
}

//----------------------------------------------------------------------------
// - Set Frozen to False
//----------------------------------------------------------------------------
void MobileObject::unfreeze()
{
    frozen_ = false;
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
// - Update
//----------------------------------------------------------------------------
// * elapsed : relative time passed since last update
//----------------------------------------------------------------------------
void MobileObject::update(float elapsed)
{
    if(!frozen_)
    {
        clock_ += speed_ * elapsed * FPS;
        while(clock_ >= 1)
        {
            step();
            clock_ -= 1;
        }
    }
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
    return (int)(ceil(distance * FPS));
}