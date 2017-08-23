#include "ActionScheduler.h"

//----------------------------------------------------------------------------
// - Action Scheduler Constructor (private)
//----------------------------------------------------------------------------
ActionScheduler::ActionScheduler() :
    AnimatedObject(FPS)
{}

//----------------------------------------------------------------------------
// - Action Scheduler Copy Constructor (private, empty)
//----------------------------------------------------------------------------
ActionScheduler::ActionScheduler(const ActionScheduler&)
{}

//----------------------------------------------------------------------------
// - Action Scheduler Destructor
//----------------------------------------------------------------------------
ActionScheduler::~ActionScheduler()
{}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Decrements each scheduled action's remaining frame delay. If any action
// reaches zero remaining frames or the action has a special trigger which is
// activated, it is executed and then removed from the schedule
//----------------------------------------------------------------------------
void ActionScheduler::step()
{
    for(auto action = schedule_.begin(); action != schedule_.end();)
    {
        if(action->countdown() < 1 || action->triggered())
        {
            action->execute();
            action = schedule_.erase(action);
        }
        else
        {
            action++;
        }
    }
}

//----------------------------------------------------------------------------
// - Access Singleton Instance
//----------------------------------------------------------------------------
ActionScheduler& ActionScheduler::instance()
{
    static ActionScheduler instance;
    return instance;
}

//----------------------------------------------------------------------------
// - Scheduler Empty?
//----------------------------------------------------------------------------
// Returns whether any actions are currently scheduled
//----------------------------------------------------------------------------
bool ActionScheduler::empty() const
{
    return schedule_.empty();
}

//----------------------------------------------------------------------------
// - Schedule Action
//----------------------------------------------------------------------------
void ActionScheduler::schedule(const Action& action)
{
    schedule_.push_back(action);
}

//----------------------------------------------------------------------------
// - Clear Schedule
//----------------------------------------------------------------------------
// Clear all scheduled tasks
//----------------------------------------------------------------------------
void ActionScheduler::clear()
{
    schedule_.clear();
}