#ifndef TACTICS_ACTOR_H
#define TACTICS_ACTOR_H

#include "MobileObject.h"
#include "../sprite/SpriteAnimated.h"
#include "../sprite/SpriteDirected.h"
#include <list>

//================================================================================
// ** Actor
//================================================================================
// Represents a character in the world as a mobile directed sprite
//================================================================================
class Actor : public MobileObject
{
// Methods
public:
    Actor(const sf::Texture& texture, const Map* ground = 0);
    virtual ~Actor();

    void                        walk(const sf::Vector2f& distance);
    void                        walkTo(const sf::Vector2f& position);
    void                        walkAlong(const std::list<sf::Vector2f>& path);
    bool                        walking() const;
    void                        stopWalking();
    std::vector<sf::Vector2f>   reach() const;
    virtual float               getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect       getGlobalBounds() const;
    void                        setTexture(const sf::Texture& sprite);
    Sprite*                     getSprite();
    const Sprite*               getSprite() const;
    void                        face(int direction);
    void                        face(const sf::Vector2f& target);
    int                         facing() const;
    
protected:
    virtual void                draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void                step();
    
// Members
    SpriteAnimated*             sprite_;
    SpriteDirected*             baseSprite_;
    std::list<sf::Vector2f>     path_;
};

#endif