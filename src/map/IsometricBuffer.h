#ifndef TACTICS_ISOMETRIC_BUFFER_H
#define TACTICS_ISOMETRIC_BUFFER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include "../objects/IsometricObject.h"
#include "IsometricNode.h"

//================================================================================
// ** IsometricBuffer
//================================================================================
// Represents a depth buffer of drawable isometric objects
//================================================================================
class IsometricBuffer : public sf::Drawable
{
public:
    IsometricBuffer(const sf::Vector3f& scale = sf::Vector3f(1, 1, 1));    
    ~IsometricBuffer();

    const sf::Vector3f& getIsometricScale() const;
    sf::Vector2f        localToIso(const sf::Vector3f& position) const;
    void                add(const IsometricObject* obj);
    void                remove(const IsometricObject* obj);
    void                isometricSort();

private:
    void                draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::set<const IsometricObject*> objects_;
    sf::Vector3f                        scale_;
};

#endif