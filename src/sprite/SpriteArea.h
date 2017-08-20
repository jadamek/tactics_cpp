#ifndef TACTICS_SPRITE_AREA_H
#define TACTICS_SPRITE_AREA_H

#include <SFML/Graphics.hpp>
#include "../player/skill/Skill.h"
#include "../objects/Actor.h"

//================================================================================
// * Sprite Area
//================================================================================
// Sprite of a single isometric area square
//================================================================================
class SpriteArea : public IsometricObject
{
// Methods
public:
    SpriteArea(const sf::Texture& texture, const sf::Color& color);
    ~SpriteArea();

    virtual sf::FloatRect   getGlobalBounds() const;
    virtual float           getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    
protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Sprite              sprite_;
};

#endif