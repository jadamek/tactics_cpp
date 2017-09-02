#ifndef TACTICS_CURSOR_H
#define TACTICS_CURSOR_H

#include "InputHandler.h"
#include "../map/Map.h"
#include "../objects/MobileObject.h"
#include <functional>

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
    Cursor(const sf::Sprite& cursor, Map& map);
    virtual ~Cursor();

    virtual void            goTo(const sf::Vector2f& position);
    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect   getGlobalBounds() const;
    virtual void            poll();
    virtual bool            busy() const;
    void                    setOnConfirm(std::function<void(const sf::Vector3f&)> action);
    void                    setOnCancel(std::function<void()> action);
    void                    setOnMove(std::function<void(const sf::Vector3f&)> action);
    
protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    sf::Sprite              sprite_;
    Map*                    map_;
    std::function<void(const sf::Vector3f&)>    actionConfirm_;
    std::function<void()>                       actionCancel_;
    std::function<void(const sf::Vector3f&)>    actionMove_;
};

#endif