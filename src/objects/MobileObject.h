#ifndef TACTICS_MOBILE_OBJECT_H
#define TACTICS_MOBILE_OBJECT_H

#include "IsometricObject.h"
#include "AnimatedObject.h"
#include "../map/Map.h"
 
//================================================================================
// ** MobileObject
//================================================================================
// Represents an isometric object which moves per-pixel through relative time
//================================================================================
class MobileObject : public IsometricObject, public AnimatedObject
{
// Methods
public:

    MobileObject(const Map* ground = 0);
    virtual ~MobileObject();

    void            moveTo(const sf::Vector3f& position);
    void            moveTo(const sf::Vector3f& position, float duration);
    bool            moving() const;
    void            stopMoving();
    float           getSpeed() const;
    void            setSpeed(float speed);

protected:
    virtual void    step();
    int             computeArrival(const sf::Vector3f& position);

// Members
    const Map*      ground_;
    int             arrival_;
    sf::Vector3f    destination_;
    float           speed_;
};

#endif