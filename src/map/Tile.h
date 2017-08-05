#ifndef TACTICS_BASIC_TILE_H
#define TACTICS_BASIC_TILE_H

#include <SFML/Graphics.hpp>
#include "../objects/IsometricObject.h"
#include "../sprite/Sprite.h"

//================================================================================
// ** Tile
//================================================================================
// Represents a single isometric map object (e.g tile, backdrop item), which
// occupies a 3-D space described by x,y,z position and height.
//================================================================================
class Tile : public IsometricObject
{
// Methods
public:
    Tile(const Sprite* sprite = 0, float height = 1.0);
    virtual ~Tile();

    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect   getGlobalBounds() const;
    const Tile*             getOccupant() const;
    Tile*                   getOccupant();
    void                    setSprite(const Sprite*);
    void                    setOccupant(Tile*);
    void                    setHeight(float);
    void                    rise(float);
    void                    lower(float);

protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    const Sprite*           sprite_;
    Tile*                   occupant_;
    float                   height_;
};

#endif