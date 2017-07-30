#ifndef TACTICS_MOBILE_OBJECT_H
#define TACTICS_MOBILE_OBJECT_H

#include "IsometricObject.h"
#include <list>
#include "../map/Map.h"
 
//================================================================================
// ** MobileObject
//================================================================================
// Represents an isometric object which moves per-pixel through relative time
//================================================================================
class MobileObject : public IsometricObject
{
public:
    MobileObject(const Map* ground = 0);
    ~MobileObject();

    float           getSpeed() const;
    void            setSpeed(float speed);
    bool            frozen() const;
    void            setFrozen(bool frozen);
    void            freeze();
    void            unfreeze();
    void            moveTo(const sf::Vector3f& position);
    void            moveAlong(const std::list<sf::Vector3f>& path);
    bool            moving() const;
    void            stopMoving();
    void            update(float elapsed);

protected:
    virtual void    step();
    int             computeArrival(const sf::Vector3f& position);

    float           speed_;
    bool            frozen_;
    const Map*      ground_;
    float           clock_;
    int             arrival_;
    std::list<sf::Vector3f> destination_;
};

#endif