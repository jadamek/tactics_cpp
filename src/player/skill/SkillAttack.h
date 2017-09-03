#ifndef TACTICS_SKILL_BASIC_ATTACK_H
#define TACTICS_SKILL_BASIC_ATTACK_H

#include "Skill.h"

//================================================================================
// ** Skill Attack
//================================================================================
// A basic attack using the player's weapon(s)
//================================================================================
class SkillAttack : public Skill
{
// Methods
public:
    SkillAttack();

    virtual void use(Actor& caster, const std::vector<Actor*>& targets);
    virtual std::vector<sf::Vector2f> range(Actor& caster) const;
    virtual std::vector<sf::Vector2f> area(const sf::Vector3f& target) const;
};

#endif