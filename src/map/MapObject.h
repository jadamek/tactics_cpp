#ifndef TACTICS_MAP_OBJECT_H
#define TACTICS_MAP_OBJECT_H

#include <SFML/Graphics.hpp>
#include "../objects/IsometricObject.h"

//================================================================================
// ** MapObject
//================================================================================
// Represents a single isometric map object (e.g tile, backdrop item), which
// occupies a 3-D space described by x,y,z position and height.
//================================================================================
class MapObject : public IsometricObject
{
// Methods
public:
    MapObject(float height = 1.0);
    virtual ~MapObject();

    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    void                    setHeight(float);
    virtual void            rise(float);
    virtual void            lower(float);

// Members
protected:
    float                   height_;
};

#endif