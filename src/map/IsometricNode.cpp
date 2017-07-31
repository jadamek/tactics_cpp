#include "IsometricBuffer.h"

//----------------------------------------------------------------------------
// - Isometric Node Contructor
//----------------------------------------------------------------------------
// * target : Drawable Isometric Object this node handles
// * container : Isometric Buffer this node belongs to
//----------------------------------------------------------------------------
IsometricNode::IsometricNode(IsometricObject* target, IsometricBuffer* container) :
    target_(target),
    container_(container),
    dirty_(true)
{
    target_->setHandler(this);
}

//----------------------------------------------------------------------------
// - Isometric Node Destructor
//----------------------------------------------------------------------------
IsometricNode::~IsometricNode()
{
    target_->setHandler(0);
    detach();
    
    if(container_)
    {
        container_->remove(target_);
    }
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

//----------------------------------------------------------------------------
// - Attach Nodes
//----------------------------------------------------------------------------
// * node : node to link to, either as a parent or child depending on sorting
// criteria
//----------------------------------------------------------------------------
void IsometricNode::attach(IsometricNode* node)
{
    if(compare(target_, node->target()))
    {
        children_.insert(node);
    }
    else{
        node->children_.insert(this);
    }
}

//----------------------------------------------------------------------------
// - Detach from Node
//----------------------------------------------------------------------------
// Empties this node's child set preparing it for re-attachment.
//----------------------------------------------------------------------------
void IsometricNode::detach()
{        
    children_.clear();
}

//----------------------------------------------------------------------------
// - Get Child Set
//----------------------------------------------------------------------------
const std::set<IsometricNode*>& IsometricNode::children() const
{
    return children_;
}

//----------------------------------------------------------------------------
// - Compare Target Priority
//----------------------------------------------------------------------------
// * a : "left" node to compare
// * b : "right" node to compare
// Returns true if a >= b, and false otherwise
//----------------------------------------------------------------------------
bool IsometricNode::compare(const IsometricObject* a, const IsometricObject* b) const
{
    // First check if either object is over the other
    if(a->position().z >= b->position().z + b->getHeight())
    {
        return true;
    }
    else if(b->position().z >= a->position().z + a->getHeight())
    {
        return false;
    }
    else
    {
        // Otherwise, compare x + y coordinates (isometric)
        if(a->position().x + a->position().y != b->position().x + b->position().y)
        {
            return a->position().x + a->position().y > b->position().x + b->position().y;
        }
        // Finally, compare y arbitrarily if isometric sums are the same
        else{
            return a->position().y >= b->position().y;
        }
    }
}

//----------------------------------------------------------------------------
// - Get Sorting Status
//----------------------------------------------------------------------------
bool IsometricNode::dirty() const
{
    dirty_;
}