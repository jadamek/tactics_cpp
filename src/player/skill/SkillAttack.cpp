#include "SkillAttack.h"
#include "../../objects/Actor.h"
#include "../../map/Map.h"

#include <iostream>
//----------------------------------------------------------------------------
// - Basic Attack Cast (Override)
//----------------------------------------------------------------------------
// * caster : player making the attack
// * targets : set of intended targets for the attack
//----------------------------------------------------------------------------
void SkillAttack::use(Actor& caster, const std::vector<Actor*>& targets)
{
    for(auto target : targets)
    {
        std::cout << caster.getName() << " ATTACKS " << target->getName() << "!!!" << std::endl;        
    }
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
}

//----------------------------------------------------------------------------
// - Calculate Basic Attack Area of Effect
//----------------------------------------------------------------------------
// * target : position where the skill is being cast onto
// Returns a vector of all positions that will be affected by a cast of the
// skill
//----------------------------------------------------------------------------
std::vector<sf::Vector2f> SkillAttack::area(const sf::Vector2f& target) const
{
    std::vector<sf::Vector2f>(1, target);
}

//----------------------------------------------------------------------------
// - Calculate Affected Targets
//----------------------------------------------------------------------------
// * target : position where the skill is being cast onto
// * map : map the intended targets of the skill inhabit
// Returns a vector of all actors caught in the area of effect for this skill
//----------------------------------------------------------------------------
std::vector<Actor*> SkillAttack::affected(const sf::Vector2f& target, const Map* map = 0) const
{
    std::vector<Actor*> targets;

    if(map)
    {
        auto aoe = area(target);
        
        for(auto position : aoe)
        {
            if(map->playerAt(position.x, position.y))
            {
                targets.push_back(map->playerAt(position.y, position.y));
            }
        }
    }
    
    return targets;
}
