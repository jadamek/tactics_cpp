#ifndef TACTICS_ACTION_SCHEDULER_H
#define TACTICS_ACTION_SCHEDULER_H

#include "Action.h"
#include "../objects/AnimatedObject.h"
#include "../settings.h"
#include <list>

//================================================================================
// ** ActionScheduler
//================================================================================
// Singleton scheduler for executing scheduled actions in relative game time
//================================================================================
class ActionScheduler : public AnimatedObject
{
// Methods
private:
    ActionScheduler();
    ActionScheduler(const ActionScheduler&);
    ~ActionScheduler();
    void                    step();

public:
    static ActionScheduler& instance();
    bool                    empty() const;
    void                    schedule(const Action& action);
    void                    clear();

// Members
private:
    std::list<Action>       schedule_;
};

#endif