#include "Action.h"
#include <algorithm>

//----------------------------------------------------------------------------
// - Action Scheduler Constructor
//----------------------------------------------------------------------------
// * action : callable to execute upon activation
// * delay : number of frames before the action executes
// * sender : object which initiated this action
//----------------------------------------------------------------------------
Action::Action(std::function<void()> action, int delay, void* sender) :
    action_(action),
    delay_(delay),
    sender_(sender),
    trigger_([](){return false;})
{}

//----------------------------------------------------------------------------
// - Action Scheduler Destructor
//----------------------------------------------------------------------------
Action::~Action()
{}

//----------------------------------------------------------------------------
// - Execute Action
//----------------------------------------------------------------------------
void Action::execute()
{
    action_();
}

//----------------------------------------------------------------------------
// - Decrement and Return Scheduling
//----------------------------------------------------------------------------
int Action::countdown()
{
    return delay_--;
}

//----------------------------------------------------------------------------
// - Get Sender
//----------------------------------------------------------------------------
void* Action::sender() const
{
    return sender_;
}

//----------------------------------------------------------------------------
// - Is Event Triggered?
//----------------------------------------------------------------------------
bool Action::triggered() const
{
    return trigger_();
}

//----------------------------------------------------------------------------
// - Set Trigger
//----------------------------------------------------------------------------
// * trigger : predicate callable which designates what condition causes this
//      action to execute alongside the duration, whichever comes first
//----------------------------------------------------------------------------
void Action::setTrigger(std::function<bool()> trigger)
{
    trigger_ = trigger;
}
