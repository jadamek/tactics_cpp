#include "Animations.h"
#include "../objects/AnimatedObject.h"

//----------------------------------------------------------------------------
// - Animations Manager Constructor (private)
//----------------------------------------------------------------------------
Animations::Animations() :
    front_(0),
    frozen_(false)
{}

//----------------------------------------------------------------------------
// - Animations Manager Copy Constructor (private, empty)
//----------------------------------------------------------------------------
Animations::Animations(const Animations& copy)
{}

//----------------------------------------------------------------------------
// - Animations Manager Destructor
//----------------------------------------------------------------------------
Animations::~Animations()
{
    Node* next, *current(front_);

    while(current)
    {
        next = current->next();
        delete current;
        current = next;
    }
}

//----------------------------------------------------------------------------
// - Get Animations Manager Single Instance
//----------------------------------------------------------------------------
Animations& Animations::instance()
{
    static Animations instance;
    return instance;
}

//----------------------------------------------------------------------------
// - Register Object to Manager
//----------------------------------------------------------------------------
// * obj : Animated object this manager will now handle
// Pushes a new animated object to the front of the list composed into a node
//----------------------------------------------------------------------------
void Animations::add(AnimatedObject* obj)
{
    if(obj)
    {
        Node* addition = new Node(obj, this, 0, front_);
        if(front_)
        {
            front_->setPrevious(addition);
        }
        
        front_ = addition;
    }
}

//----------------------------------------------------------------------------
// - Freeze Animations
//----------------------------------------------------------------------------
void Animations::freeze()
{
    frozen_ = true;
}

//----------------------------------------------------------------------------
// - Unfreeze Animations
//----------------------------------------------------------------------------
void Animations::unfreeze()
{
    frozen_ = false;
}

//----------------------------------------------------------------------------
// - Set Frozen Status
//----------------------------------------------------------------------------
void Animations::setFrozen(bool frozen)
{
    frozen_ = frozen;
}

//----------------------------------------------------------------------------
// - Animations Frozen?
//----------------------------------------------------------------------------
bool Animations::frozen() const
{
    frozen_;
}

//----------------------------------------------------------------------------
// - Update Animations
//----------------------------------------------------------------------------
void Animations::update(float elapsed)
{
    Node* node = front_;

    while(node && !frozen_)
    {
        node->getObject()->update(elapsed);
        node = node->next();
    }
}
