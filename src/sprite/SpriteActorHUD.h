#ifndef TACTICS_SPRITE_ACTOR_HUD_H
#define TACTICS_SPRITE_ACTOR_HUD_H

#include "../objects/Actor.h"
#include "Sprite.h"

//================================================================================
// ** Sprite Actor HUD
//================================================================================
// Displays the actor's portrait, name and other vital stats in an overlay HUD
//================================================================================
class SpriteActorHUD : public Sprite
{
// Methods
public:
    SpriteActorHUD();
    SpriteActorHUD(const Actor* actor);
    virtual ~SpriteActorHUD();

    void                setActor(const Actor* actor);
    bool                visible() const;
    void                show();
    void                hide();
    sf::FloatRect       getGlobalBounds() const;
    
    
protected:
    virtual void        draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    sf::Sprite*         portrait_;
    sf::RectangleShape  body_;
    sf::Text            name_;
    sf::Text            lvl_;
    sf::Text            hpLabel_;
    sf::Text            hpValue_;
    sf::Text            mpLabel_;
    sf::Text            mpValue_;
    sf::Font            font_;
    bool                visible_;
    bool                empty_;
};

#endif
