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
    std::cout << "ATTACK!!!" << std::endl;
}

//----------------------------------------------------------------------------
// - Calculate Basic Attack Range
//----------------------------------------------------------------------------
// * caster : player making the attack
// Returns a vector of possible positions the skill may be cast at
//----------------------------------------------------------------------------
std::vector<sf::Vector2i> SkillAttack::range(Actor& caster) const
{
    std::vector<sf::Vector2i> result;

    if(caster.getEnvironment()->valid(caster.position().x + 1, caster.position().y))
    {
        result.push_back(sf::Vector2i(caster.position().x + 1, caster.position().y));
    }

    if(caster.getEnvironment()->valid(caster.position().x - 1, caster.position().y))
    {
        result.push_back(sf::Vector2i(caster.position().x - 1, caster.position().y));
    }

    if(caster.getEnvironment()->valid(caster.position().x, caster.position().y + 1))
    {
        result.push_back(sf::Vector2i(caster.position().x, caster.position().y + 1));
    }

    if(caster.getEnvironment()->valid(caster.position().x, caster.position().y - 1))
    {
        result.push_back(sf::Vector2i(caster.position().x, caster.position().y - 1));
    }
}

//----------------------------------------------------------------------------
// - Calculate Basic Attack Area of Effect
//----------------------------------------------------------------------------
// * source : position where the skill is being cast from
// * map : environment where the aoe of the skill is being computed for
// Returns a vector of all positions that will be affected by a cast of the
// skill
//----------------------------------------------------------------------------
std::vector<sf::Vector2i> SkillAttack::area(const sf::Vector2i& source, Map& map) const
{
    std::vector<sf::Vector2i>(1, sf::Vector2i(source));
}