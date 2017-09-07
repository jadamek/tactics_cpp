#include "Skill.h"
#include "../../map/Map.h"
#include "../../objects/Actor.h"

//----------------------------------------------------------------------------
// Skill Constructor
//----------------------------------------------------------------------------
Skill::Skill(Actor& caster) : 
    caster_(&caster),
    name_("SKILL"),
    casting_(false)
{}

//----------------------------------------------------------------------------
// Skill Constructor
//----------------------------------------------------------------------------
Skill::~Skill()
{}

//----------------------------------------------------------------------------
// Get Skill's Caster
//----------------------------------------------------------------------------
Actor* Skill::caster()
{
    return caster_;
}
//----------------------------------------------------------------------------
// Is Skill Casting?
//----------------------------------------------------------------------------
bool Skill::casting() const
{
    return casting_;
}

//----------------------------------------------------------------------------
// Get Skill's Name
//----------------------------------------------------------------------------
std::string Skill::name() const
{
    return name_;
}

//----------------------------------------------------------------------------
// Set Casting Flag
//----------------------------------------------------------------------------
// * casting : flag indicating the spell is currently being cast
//----------------------------------------------------------------------------
void Skill::setCastingStatus(bool casting)
{
    casting_ = casting;
}
#include <iostream>
//----------------------------------------------------------------------------
// - Calculate Affected Targets
//----------------------------------------------------------------------------
// * target : position where the skill is being cast onto
// Returns a vector of all actors caught in the area of effect for this skill
//----------------------------------------------------------------------------
std::vector<Actor*> Skill::affected(const sf::Vector3f& target) const
{
    std::vector<Actor*> targets;
    const Map* map = caster_->getEnvironment();
    Actor* victim;

    if(map)
    {
        auto aoe = area(target);
        
        for(auto position : aoe)
        {                    
            if((victim = map->playerAt(position.x, position.y)))
            {
                targets.push_back(victim);
            }
        }
    }
    
    return targets;
}

//----------------------------------------------------------------------------
// - Is Effective?
//----------------------------------------------------------------------------
// * target : position where the skill is being cast onto
// Returns TRUE if at least one actor is affected by a cast of this skill at
// the specified target position
//----------------------------------------------------------------------------
bool Skill::effective(const sf::Vector3f& target) const
{
    if(caster_->getEnvironment())
    {
        auto aoe = area(target);
        
        for(auto position : aoe)
        {
            if(caster_->getEnvironment()->playerAt(position.x, position.y))
            {
                return true;
            }
        }
    }
    
    return false;
}