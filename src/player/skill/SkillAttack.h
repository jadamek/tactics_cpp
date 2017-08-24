#ifndef TACTICS_SKILL_BASIC_ATTACK_H
#define TACTICS_SKILL_BASIC_ATTACK_H

#include "Skill.h"

//================================================================================
// ** Skill Attack
//================================================================================
// A basic attack using the player's weapon(s)
//================================================================================
class SkillAttack : Skill
{
// Methods
    virtual void use(Actor& caster, const std::vector<Actor*>& targets);
    virtual std::vector<sf::Vector2i> range(Actor& caster) const;
    virtual std::vector<sf::Vector2i> area(const sf::Vector2i& source, Map& map) const;
};

#endif