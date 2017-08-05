#ifndef TACTICS_SPRITE_BASIC_TILE_H
#define TACTICS_SPRITE_BASIC_TILE_H

#include <SFML/Graphics.hpp>
#include "../Sprite.h"

//================================================================================
// ** SpriteTile
//================================================================================
// A sprite of a single (continuous or discrete) tile column.
//================================================================================
class SpriteTile : public Sprite{
// Methods
public:
    SpriteTile(const sf::Texture&, float width = 0, float length = 0, float height = 0, bool continuous = false);
    virtual ~SpriteTile();

    sf::FloatRect   getGlobalBounds() const;
    void            resetHeight(float);

protected:
    void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    float           width_;
    float           length_;
    float           height_;
    bool            continuous_;
    sf::Sprite*     top_;
    sf::Sprite*     body_;
    sf::Sprite*     bottom_;
};

#endif