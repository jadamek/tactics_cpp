#ifndef TACTICS_SPRITE_AREA_H
#define TACTICS_SPRITE_AREA_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "SpriteAreaSquare.h"
#include "../map/Map.h"

//================================================================================
// * Sprite Area Square
//================================================================================
// Compound sprite of an area of interest
//================================================================================
class SpriteArea : public IsometricObject
{
// Methods
public:
    SpriteArea(const sf::Texture& texture, const std::vector<sf::Vector2f>& area, const Map& map, const sf::Color& color);
    virtual ~SpriteArea();

    virtual sf::FloatRect   getGlobalBounds() const;
    virtual void            join(IsometricBuffer* buffer) const;
    bool                    contains(const sf::Vector2f& position) const;
    
protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    std::vector<SpriteAreaSquare> area_;    
};

#endif

