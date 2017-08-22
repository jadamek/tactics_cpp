#ifndef TACTICS_SPRITE_AREA_SQUARE_H
#define TACTICS_SPRITE_AREA_SQUARE_H

#include <SFML/Graphics.hpp>
#include "../objects/IsometricObject.h"

//================================================================================
// * Sprite Area Square
//================================================================================
// Sprite of a single isometric area square
//================================================================================
class SpriteAreaSquare : public IsometricObject
{
// Methods
public:
    SpriteAreaSquare(const sf::Texture& texture, const sf::Color& color);
    virtual ~SpriteAreaSquare();

    virtual sf::FloatRect   getGlobalBounds() const;
    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    
protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Sprite              sprite_;
};

#endif