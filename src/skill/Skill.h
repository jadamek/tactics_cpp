#ifndef TACTICS_PLAYER_SKILL_H
#define TACTICS_PLAYER_SKILL_H

#include<vector>

class Actor;

//================================================================================
// ** Skill
//================================================================================
// Abstract base for all usable skills, ranging from basic attacks to cast magic
//================================================================================
class Skill
{
// Methods
public:
    virtual void use(Actor* caster, const std::vector<Actor*>& targets) = 0;
};

#endif