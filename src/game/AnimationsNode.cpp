#include "Animations.h"
#include "../objects/AnimatedObject.h"

//----------------------------------------------------------------------------
// - Animations Manager Node Constructor
//----------------------------------------------------------------------------
Animations::Node::Node(AnimatedObject* obj, Animations* container, Node* prev, Node* next) :
    object_(obj),
    container_(container),
    prev_(prev),
    next_(next)
{
    if(obj)
    {
        obj->animHandler_ = this;
    }
}

//----------------------------------------------------------------------------
// - Animations Manager Node Destructor
//----------------------------------------------------------------------------
Animations::Node::~Node()
{
    if(next_)
    {
        next_->setPrevious(prev_);
    }

    if(prev_)
    {
        prev_->setNext(next_);
    }
    else
    {
        if(container_->front_ == this)
        {
            container_->front_ = next_;
        }
    }

    object_->animHandler_ = 0;
}

//----------------------------------------------------------------------------
// - Get Previous Node
//----------------------------------------------------------------------------
Animations::Node* Animations::Node::previous()
{
    return prev_;
}

//----------------------------------------------------------------------------
// - Set Previous Node
//----------------------------------------------------------------------------
// * prev : previous node in the container
//----------------------------------------------------------------------------
void Animations::Node::setPrevious(Node* prev)
{
    prev_ = prev;
}

//----------------------------------------------------------------------------
// - Get Next Node
//----------------------------------------------------------------------------
Animations::Node* Animations::Node::next()
{
    return next_;
}

//----------------------------------------------------------------------------
// - Set Next Node
//----------------------------------------------------------------------------
// * next : next node in the container
//----------------------------------------------------------------------------
void Animations::Node::setNext(Node* next)
{
    next_ = next;
}

//----------------------------------------------------------------------------
// - Get Target Object (Mutable)
//----------------------------------------------------------------------------
AnimatedObject* Animations::Node::getObject()
{
    return object_;
}

//----------------------------------------------------------------------------
// - Get Target Object
//----------------------------------------------------------------------------
const AnimatedObject* Animations::Node::getObject() const
{
    return object_;    
}