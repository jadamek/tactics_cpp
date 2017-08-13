#ifndef TACTICS_BASIC_TILE_H
#define TACTICS_BASIC_TILE_H

#include <SFML/Graphics.hpp>
#include "MapObject.h"
#include "../sprite/Sprite.h"

//================================================================================
// ** Tile
//================================================================================
// Represents a single basic isometric map tile
//================================================================================
class Tile : public MapObject
{
// Methods
public:
    Tile(const Sprite* sprite = 0, float height = 1.0);
    virtual ~Tile();

    virtual sf::FloatRect   getGlobalBounds() const;
    const MapObject*        getOccupant() const;
    MapObject*              getOccupant();
    void                    setSprite(const Sprite*);
    void                    setOccupant(MapObject*);
    virtual void            rise(float);
    virtual void            lower(float);

protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    const Sprite*           sprite_;
    MapObject*              occupant_;
};

#endif