#ifndef TACTICS_ACTOR_H
#define TACTICS_ACTOR_H

#include "MobileObject.h"
#include "../sprite/SpriteAnimated.h"
#include "../sprite/SpriteDirected.h"
#include "../player/skill/Skill.h"
#include <deque>

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
    void                        walkAlong(const std::deque<sf::Vector2f>& path);
    bool                        walking() const;
    void                        stopWalking();
    std::vector<sf::Vector2f>   reach() const;
    std::deque<sf::Vector2f>    shortestPath(const sf::Vector2f& destination) const;
    virtual float               getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect       getGlobalBounds() const;
    void                        setTexture(const sf::Texture& sprite);
    Sprite*                     getSprite();
    const Sprite*               getSprite() const;
    void                        face(int direction);
    void                        face(const sf::Vector2f& target);
    int                         facing() const;
    const Map*                  getEnvironment() const;
    sf::Sprite*                 getPortrait() const;
    void                        setPortrait(const sf::Texture& portrait);
    const std::string&          getName() const; 
    void                        setName(const std::string& name);
    int                         getMove() const;
    void                        setMove(int mv);
    void                        focus();
    void                        unfocus();
    
protected:
    virtual void                draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void                step();
    virtual bool                occupiable(const sf::Vector2f& position) const;
    virtual bool                passable(const sf::Vector2f& from, const sf::Vector2f& to) const;    
    
// Members
    SpriteAnimated*             sprite_;
    SpriteDirected*             baseSprite_;
    std::deque<sf::Vector2f>    path_;
    sf::Sprite*                 portrait_;
    std::string                 name_;
    int                         attrMove_;
    std::vector<Skill*>         skills_;
};

#endif