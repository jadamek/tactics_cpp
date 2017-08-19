#ifndef TACTICS_SPRITE_MENU_FRAME_H
#define TACTICS_SPRITE_MENU_FRAME_H

#include <SFML/Graphics.hpp>
#include "Sprite.h"

//================================================================================
// ** Sprite Menu Frame
//================================================================================
// A sprite depiciting the frame of a menu (empty body)
//================================================================================
class SpriteMenuFrame : public Sprite
{
// Methods
public:
    SpriteMenuFrame(const sf::Texture& texture, sf::Vector2u size = sf::Vector2u(32, 32));
    ~SpriteMenuFrame();

    virtual sf::FloatRect   getGlobalBounds() const;    
    
protected:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
// Members
    sf::Sprite*             frame_;
    int                     width_x;
    int                     width_y;
};

#endif