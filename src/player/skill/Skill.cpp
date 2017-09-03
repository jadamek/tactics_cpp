#include "Skill.h"
#include "../../map/Map.h"
#include "../../objects/Actor.h"

//----------------------------------------------------------------------------
// Skill Constructor
//----------------------------------------------------------------------------
Skill::Skill() : 
    name_("SKILL"),
    casting_(false)
{}

//----------------------------------------------------------------------------
// Skill Constructor
//----------------------------------------------------------------------------
Skill::~Skill()
{}

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
// * map : map the intended targets of the skill inhabit
// Returns a vector of all actors caught in the area of effect for this skill
//----------------------------------------------------------------------------
std::vector<Actor*> Skill::affected(const sf::Vector3f& target, const Map* map) const
{
    std::vector<Actor*> targets;

    if(map)
    {
        auto aoe = area(target);
        
        for(auto position : aoe)
        {                    
            if(map->playerAt(position.x, position.y))
            {
                targets.push_back(map->playerAt(position.x, position.y));
            }
        }
    }
    
    return targets;
}

//----------------------------------------------------------------------------
// - Is Effective?
//----------------------------------------------------------------------------
// * target : position where the skill is being cast onto
// * map : map the intended targets of the skill inhabit
// Returns TRUE if at least one actor is affected by a cast of this skill at
// the specified target position
//----------------------------------------------------------------------------
bool Skill::effective(const sf::Vector3f& target, const Map* map) const
{
    if(map)
    {
        auto aoe = area(target);
        
        for(auto position : aoe)
        {
            if(map->playerAt(position.x, position.y))
            {
                return true;
            }
        }
    }
    
    return false;
}