#ifndef TACTICS_ANIMATED_OBJECT_H
#define TACTICS_ANIMATED_OBJECT_H

#include "../settings.h"
#include "../game/Animations.h"

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
    virtual ~AnimatedObject();

    float           getFPS() const;
    void            setFPS(float fps);
    bool            frozen() const;
    void            setFrozen(bool frozen);
    void            freeze();
    void            unfreeze();
    void            update(float elapsed);

    friend class Animations::Node;

protected:
    virtual void    step() = 0;

// Members
    float           fps_;
    float           clock_;
    bool            frozen_;

private:
    Animations::Node* animHandler_; 
};

#endif