#ifndef TACTICS_TARGET_CONFIRMER_H
#define TACTICS_TARGET_CONFIRMER_H

#include "../player/skill/Skill.h"
#include "../objects/Actor.h"
#include "InputHandler.h"
#include "../objects/MobileObject.h"
#include "../sprite/SpriteArea.h"
#include <vector>
#include <functional>

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
    TargetConfirmer(const sf::Sprite& cursor, Skill& skill, Actor& caster, const sf::Vector2f& target);
    virtual ~TargetConfirmer();

    virtual float               getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect       getGlobalBounds() const;
    virtual void                poll();
    virtual bool                busy() const;
    void                        setOnConfirm(std::function<void()> action);
    void                        setOnCancel(std::function<void()> action);
    void                        setOnMove(std::function<void(Actor*)> action);    
    
protected:
    virtual void                draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    sf::Sprite                  sprite_;
    SpriteArea**                targetArea_;
    Skill*                      skill_;
    Actor*                      caster_;
    const std::vector<Actor*>&  targets_;
    int                         current_;
    std::function<void()>       actionConfirm_;
    std::function<void()>       actionCancel_;
    std::function<void(Actor*)> actionMove_;    
};


#endif