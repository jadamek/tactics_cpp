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
// Decrements each scheduled task's remaining frame delay. If any action
// reaches zero remaining frames, it is executed and then removed from the
// schedule
//----------------------------------------------------------------------------
void ActionScheduler::step()
{
    for(auto task = schedule_.begin(); task != schedule_.end();)
    {
        if(task->second-- < 1)
        {
            task->first();
            task = schedule_.erase(task);
        }
        else
        {
            task++;
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
// - 
//----------------------------------------------------------------------------
void ActionScheduler::schedule(void(*action)(), int delay)
{
    if(delay >= 0)
    {
        schedule_.push_back(std::pair<void(*&)(), int>(action, delay));
    }
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