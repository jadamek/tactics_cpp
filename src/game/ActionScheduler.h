#ifndef TACTICS_ACTION_SCHEDULER_H
#define TACTICS_ACTION_SCHEDULER_H

#include "../objects/AnimatedObject.h"
#include "../settings.h"
#include <list>
#include <functional>

//================================================================================
// ** ActionScheduler
//================================================================================
// Singleton scheduler for executing scheduled callback functions in relative game
// time
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
    void                    schedule(std::function<void()>, int delay);
    void                    clear();

// Members
private:
    std::list<std::pair<std::function<void()>, int>> schedule_;
};

#endif