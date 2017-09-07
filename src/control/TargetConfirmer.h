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
    TargetConfirmer(const sf::Sprite& cursor, Skill& skill, const sf::Vector3f& target);
    virtual ~TargetConfirmer();

    virtual float               getHeight(const sf::Vector2f& position = sf::Vector2f(0, 0)) const;
    virtual sf::FloatRect       getGlobalBounds() const;
    virtual void                poll();
    virtual bool                busy() const;
    void                        setOnConfirm(std::function<void()> action);
    void                        setOnCancel(std::function<void()> action);
    void                        setOnMove(std::function<void(Actor*)> action);
    const std::vector<Actor*>&  getTargets() const;
    void                        setTargets(Skill& skill, const sf::Vector3f& target);
    
protected:
    virtual void                draw(sf::RenderTarget& target, sf::RenderStates states) const;

// Members
    sf::Sprite                  sprite_;
    Skill*                      skill_;
    std::vector<Actor*>         targets_;
    int                         current_;
    std::function<void()>       actionConfirm_;
    std::function<void()>       actionCancel_;
    std::function<void(Actor*)> actionMove_;    
};


#endif