#ifndef TACTICS_MOBILE_OBJECT_H
#define TACTICS_MOBILE_OBJECT_H

#include "IsometricObject.h"
#include "AnimatedObject.h"
#include "../map/Map.h"
#include <list>
 
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
    ~MobileObject();

    void            moveTo(const sf::Vector3f& position);
    void            moveAlong(const std::list<sf::Vector3f>& path);
    bool            moving() const;
    void            stopMoving();

protected:
    virtual void    step();
    int             computeArrival(const sf::Vector3f& position);

// Members
    const Map*              ground_;
    int                     arrival_;
    std::list<sf::Vector3f> destination_;
};

#endif