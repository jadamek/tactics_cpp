#include "IsometricNode.h"

//----------------------------------------------------------------------------
// - Isometric Node Contructor
//----------------------------------------------------------------------------
// * target : Drawable Isometric Object this node handles
//----------------------------------------------------------------------------
IsometricNode::IsometricNode(IsometricObject* target) :
    target_(target),
    dirty_(false)
{
    target_->setHandler(this);
}

//----------------------------------------------------------------------------
// - Isometric Node Destructor
//----------------------------------------------------------------------------
IsometricNode::~IsometricNode()
{
    target_->setHandler(0);
}

//----------------------------------------------------------------------------
// - Get Target Object
//----------------------------------------------------------------------------
const IsometricObject* IsometricNode::target() const
{
    return target_;
}

//----------------------------------------------------------------------------
// - Get Target Object (Mutable)
//----------------------------------------------------------------------------
IsometricObject* IsometricNode::target()
{
    return target_;
}

//----------------------------------------------------------------------------
// - Alert of Changed Status
//----------------------------------------------------------------------------
// Sets the dirty_ flag to indicate a necessity for object re-sorting
//----------------------------------------------------------------------------
void IsometricNode::alert()
{
    dirty_ = true;
}