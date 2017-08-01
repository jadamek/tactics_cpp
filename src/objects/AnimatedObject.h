#ifndef TACTICS_ANIMATED_OBJECT_H
#define TACTICS_ANIMATED_OBJECT_H

#include "../settings.h"

//================================================================================
// ** AnimatedObject
//================================================================================
// Represents an abstract object animated in relative time, calling a step()
// method per frame based on a relative (mutable) FPS rate
// update() progresses this relative time
//================================================================================
class AnimatedObject
{
// Methods
public:
    AnimatedObject(float fps = FPS);

    float           getSpeed() const;
    void            setSpeed(float speed);
    float           getFPS() const;
    void            setFPS(float fps);
    bool            frozen() const;
    void            setFrozen(bool frozen);
    void            freeze();
    void            unfreeze();
    virtual void    update(float elapsed);

protected:
    virtual void    step() = 0;

// Members
    float           fps_;
    float           speed_;
    float           clock_;
    bool            frozen_;
};

#endif