#ifndef TACTICS_ACTION_H
#define TACTICS_ACTION_H

#include <functional>

//================================================================================
// ** Action
//================================================================================
// Represents an action that may be scheduled to occur in a number of frames or by
// a specific Boolean trigger condition
//================================================================================
class Action
{
// Methods
public:
    Action(std::function<void()> action, int delay, void* sender = 0);
    virtual ~Action();

    void    execute();
    int     countdown();
    void*   sender() const;
    bool    triggered() const;
    void    setTrigger(std::function<bool()> trigger);

protected:
    std::function<void()>   action_;
    int                     delay_;
    void*                   sender_;
    std::function<bool()>   trigger_;
};

#endif