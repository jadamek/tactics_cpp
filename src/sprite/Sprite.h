#ifndef TACTICS_BASE_SPRITE_H
#define TACTICS_BASE_SPRITE_H

#include <SFML/Graphics.hpp>

//================================================================================
// ** Sprite
//================================================================================
// An abstract sprite which combines the Drawable and Transformable interfaces, as
// well as adds all basic interfaces for sprites.
// * NOTE: sf::Sprite will be composed within this implementation, not extended
//================================================================================
class Sprite : public sf::Drawable, public sf::Transformable{
public:
    virtual sf::FloatRect       getGlobalBounds() const = 0;
};

#endif