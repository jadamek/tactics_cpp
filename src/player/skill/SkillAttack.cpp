#include "SkillAttack.h"
#include "../../objects/Actor.h"
#include "../../map/Map.h"

#include <iostream>

//----------------------------------------------------------------------------
// Skill Constructor
//----------------------------------------------------------------------------
SkillAttack::SkillAttack(Actor& caster) :
    Skill(caster)
{
    name_ = "Attack";
}
    
//----------------------------------------------------------------------------
// - Basic Attack Cast (Override)
//----------------------------------------------------------------------------
// * targets : set of intended targets for the attack
//----------------------------------------------------------------------------
void SkillAttack::use(const std::vector<Actor*>& targets)
{
    setCastingStatus(true);

    for(auto target : targets)
    {
        std::cout << caster_->getName() << " ATTACKS " << target->getName() << "!!!" << std::endl;        
    }

    setCastingStatus(false);    
}

//----------------------------------------------------------------------------
// - Calculate Basic Attack Range
//----------------------------------------------------------------------------
// Returns a vector of possible positions the skill may be cast at
//----------------------------------------------------------------------------
std::vector<sf::Vector2f> SkillAttack::range() const
{
    std::vector<sf::Vector2f> result;

    if(caster_->getEnvironment()->valid(caster_->position().x + 1, caster_->position().y))
    {
        result.push_back(sf::Vector2f(caster_->position().x + 1, caster_->position().y));
    }

    if(caster_->getEnvironment()->valid(caster_->position().x - 1, caster_->position().y))
    {
        result.push_back(sf::Vector2f(caster_->position().x - 1, caster_->position().y));
    }

    if(caster_->getEnvironment()->valid(caster_->position().x, caster_->position().y + 1))
    {
        result.push_back(sf::Vector2f(caster_->position().x, caster_->position().y + 1));
    }

    if(caster_->getEnvironment()->valid(caster_->position().x, caster_->position().y - 1))
    {
        result.push_back(sf::Vector2f(caster_->position().x, caster_->position().y - 1));
    }

    return result;
}

//----------------------------------------------------------------------------
// - Calculate Basic Attack Area of Effect
//----------------------------------------------------------------------------
// * target : position where the skill is being cast onto
// Returns a vector of all positions that will be affected by a cast of the
// skill
//----------------------------------------------------------------------------
std::vector<sf::Vector2f> SkillAttack::area(const sf::Vector3f& target) const
{
    return std::vector<sf::Vector2f>(1, sf::Vector2f(target.x, target.y));
}

