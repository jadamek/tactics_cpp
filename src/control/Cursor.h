#ifndef TACTICS_CURSOR_H
#define TACTICS_CURSOR_H

#include "../game/InputHandler.h"
#include "../map/Map.h"
#include "../map/MapObject.h"
#include "../objects/MobileObject.h"
#include "../objects/IsometricObject.h"

//================================================================================
// ** Cursor
//================================================================================
// Represents a map cursor which handles input events and selects players on the
// map as well as the MapObject of its current position
//================================================================================
class Cursor : public MobileObject, public InputHandler
{
// Methods
public:
    Cursor(const sf::Texture& texture, Map* map, InputHandler* parent = 0);
    virtual ~Cursor();

    void                    goTo(const sf::Vector2f& position);
    Actor*                  select() const;
    MapObject*              getEnvironment() const;    
    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect   getGlobalBounds() const;
    virtual void            poll();
    virtual bool            busy() const;    
    
protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    sf::Sprite              sprite_;
    Map*                    map_;
};

#endif