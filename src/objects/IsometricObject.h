#ifndef TACTICS_ISOMETRIC_OBJECT_H
#define TACTICS_ISOMETRIC_OBJECT_H

#include <SFML/Graphics.hpp>

class IsometricNode;

//================================================================================
// ** IsometricObject
//================================================================================
// Represents a drawable object in the isometric space.
//================================================================================
class IsometricObject : public sf::Drawable
{
public:
    IsometricObject();
    ~IsometricObject();
    
    const sf::Vector3f&     position() const;
    virtual float           getHeight(const sf::Vector2f& position) const;
    virtual sf::FloatRect   getGlobalBounds() const = 0;
    void                    setPosition(const sf::Vector3f& position);
    void                    setHandler(IsometricNode*);
    void                    removeHandler();

protected: 
    sf::Vector3f            position_;
    IsometricNode*          handler_;
};

#endif