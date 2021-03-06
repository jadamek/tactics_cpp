#ifndef TACTICS_ISOMETRIC_OBJECT_H
#define TACTICS_ISOMETRIC_OBJECT_H

#include <SFML/Graphics.hpp>

class IsometricNode;
class IsometricBuffer;

//================================================================================
// ** IsometricObject
//================================================================================
// Represents a drawable object in the isometric space.
//================================================================================
class IsometricObject : public sf::Drawable
{
// Methods    
public:
    IsometricObject();
    virtual ~IsometricObject();
    
    const sf::Vector3f&     position() const;
    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect   getGlobalBounds() const = 0;
    void                    setPosition(const sf::Vector3f& position);
    sf::Vector2f            getGlobalPosition() const;
    virtual void            join(IsometricBuffer* buffer) const;

    static sf::Vector2f     isoToGlobal(const sf::Vector3f& position);
    
    friend class            IsometricNode;
    friend class            IsometricBuffer;

private:
    IsometricNode*          getHandler() const;
    void                    setHandler(IsometricNode*);

// Members
protected: 
    sf::Vector3f            position_;
    IsometricNode*          handler_;
};

#endif