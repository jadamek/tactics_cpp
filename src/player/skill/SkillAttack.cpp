#include "SkillAttack.h"
#include "../../objects/Actor.h"
#include "../../map/Map.h"

#include <iostream>

//----------------------------------------------------------------------------
// Skill Constructor
//----------------------------------------------------------------------------
SkillAttack::SkillAttack()
{
    name_ = "Attack";
}
    
//----------------------------------------------------------------------------
// - Basic Attack Cast (Override)
//----------------------------------------------------------------------------
// * caster : player making the attack
// * targets : set of intended targets for the attack
//----------------------------------------------------------------------------
void SkillAttack::use(Actor& caster, const std::vector<Actor*>& targets)
{
    setCastingStatus(true);

    for(auto target : targets)
    {
        std::cout << caster.getName() << " ATTACKS " << target->getName() << "!!!" << std::endl;        
    }

    setCastingStatus(false);    
}

//----------------------------------------------------------------------------
// - Calculate Basic Attack Range
//----------------------------------------------------------------------------
// * caster : player making the attack
// Returns a vector of possible positions the skill may be cast at
//----------------------------------------------------------------------------
std::vector<sf::Vector2f> SkillAttack::range(Actor& caster) const
{
    std::vector<sf::Vector2f> result;

    if(caster.getEnvironment()->valid(caster.position().x + 1, caster.position().y))
    {
        result.push_back(sf::Vector2f(caster.position().x + 1, caster.position().y));
    }

    if(caster.getEnvironment()->valid(caster.position().x - 1, caster.position().y))
    {
        result.push_back(sf::Vector2f(caster.position().x - 1, caster.position().y));
    }

    if(caster.getEnvironment()->valid(caster.position().x, caster.position().y + 1))
    {
        result.push_back(sf::Vector2f(caster.position().x, caster.position().y + 1));
    }

    if(caster.getEnvironment()->valid(caster.position().x, caster.position().y - 1))
    {
        result.push_back(sf::Vector2f(caster.position().x, caster.position().y - 1));
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

