#include "IsometricBuffer.h"
#include <algorithm>

//----------------------------------------------------------------------------
// - Isometric Node Contructor
//----------------------------------------------------------------------------
// * target : Drawable Isometric Object this node handles
// * container : Isometric Buffer this node belongs to
//----------------------------------------------------------------------------
IsometricNode::IsometricNode(IsometricObject* target, IsometricBuffer* container) :
    target_(target),
    container_(container),
    dirty_(true),
    visited_(0)    
{
    target_->setHandler(this);

    if(target_ && container_)
    {
        // Compute bounding box
        sf::FloatRect bounds = target_->getGlobalBounds();
        sf::Vector2f isometric_position = target_->getGlobalPosition();
        bounds.left += isometric_position.x;
        bounds.top += isometric_position.y;

        setBounds(bounds);
    }
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

    if(container_)
    {
        // Re-compute bounding box
        sf::FloatRect bounds = target_->getGlobalBounds();
        sf::Vector2f isometric_position = target_->getGlobalPosition();
        bounds.left += isometric_position.x;
        bounds.top += isometric_position.y;

        setBounds(bounds);

        // Alert buffer of need for re-sort
        container_->alert();
    }
}

//----------------------------------------------------------------------------
// - Resolve Sorting Status
//----------------------------------------------------------------------------
// Clears the dirty_ flag to indicate object re-sorting resolved
//----------------------------------------------------------------------------
void IsometricNode::resolve()
{
    dirty_ = false;
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
        children_.push_back(node);
    }
    else{
        node->children_.push_back(this);
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
// - Remove Child Node
//----------------------------------------------------------------------------
// * child : child node to be removed from children set
//----------------------------------------------------------------------------
void IsometricNode::removeChild(IsometricNode* child)
{
    auto child_it = std::find(children_.begin(), children_.end(), child);
    if(child_it != children_.end())
    {
        children_.erase(child_it);        
    }
}

//----------------------------------------------------------------------------
// - Get Child Set
//----------------------------------------------------------------------------
const std::deque<IsometricNode*>& IsometricNode::children() const
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
    if(a->position().z >= b->position().z + b->getHeight(sf::Vector2f(a->position().x - b->position().x, a->position().y - b->position().y)))
    {
        return true;
    }
    else if(b->position().z >= a->position().z + a->getHeight(sf::Vector2f(a->position().x - b->position().x, a->position().y - b->position().y)))
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
    return dirty_;
}

//----------------------------------------------------------------------------
// - Get Bounding Box
//----------------------------------------------------------------------------
const sf::FloatRect& IsometricNode::getBounds() const
{
    return bounds_;
}

//----------------------------------------------------------------------------
// - Set Bounding Box
//----------------------------------------------------------------------------
// * bounds : computed bounding box for this node during isometric sorting
// Used by isometric sort in IsometricBuffer as a temporary store
//----------------------------------------------------------------------------
void IsometricNode::setBounds(const sf::FloatRect& bounds)
{
    bounds_ = bounds;
}

//----------------------------------------------------------------------------
// - Remove Node from Buffer for Deletion
//----------------------------------------------------------------------------
void IsometricNode::deactivate()
{        
    if(container_)
    {
        container_->remove(this);
    }
}

//----------------------------------------------------------------------------
// - Get Visited Status
//----------------------------------------------------------------------------
int IsometricNode::visited() const
{
    return visited_;
}

//----------------------------------------------------------------------------
// - Set Visited Status
//----------------------------------------------------------------------------
// * visited : visitation status for depth-first traversal during topological
//      sorting procedure
//----------------------------------------------------------------------------
void IsometricNode::setVisited(bool visited)
{
    visited_ = visited;
}