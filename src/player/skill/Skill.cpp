#include "Skill.h"

//----------------------------------------------------------------------------
// Skill Constructor
//----------------------------------------------------------------------------
Skill::Skill() : 
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
// Set Casting Flag
//----------------------------------------------------------------------------
// * casting : flag indicating the spell is currently being cast
//----------------------------------------------------------------------------
void Skill::setCastingStatus(bool casting)
{
    casting_ = casting;
}