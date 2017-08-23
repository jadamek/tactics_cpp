#ifndef TACTICS_TARGET_CONFIRMER_H
#define TACTICS_TARGET_CONFIRMER_H

#include "../player/skill/Skill.h"
#include "../objects/Actor.h"
#include "../game/InputHandler.h"
#include "../objects/MobileObject.h"
#include <vector>

//================================================================================
// ** TargetConfirmer
//================================================================================
// Specialized cursor which is used to review and confirm a target selection for
// skill casting
//================================================================================
class TargetConfirmer : public MobileObject, public InputHandler
{
// Methods
public:
    TargetConfirmer(const sf::Texture& texture, Skill* skill, Actor* caster, const std::vector<Actor*>& targets);
    virtual ~TargetConfirmer();

    virtual float               getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect       getGlobalBounds() const;
    virtual void                poll();
    virtual bool                busy() const;
    
protected:
    virtual void                draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void                step();

// Members
    sf::Sprite                  sprite_;
    int                         throttle_;
    Skill*                      skill_;
    Actor*                      caster_;
    const std::vector<Actor*>&  targets_;
    int                         current_;
};


#endif