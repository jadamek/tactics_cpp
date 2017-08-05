#include "AnimatedObject.h"

//----------------------------------------------------------------------------
// - Animated Object Constructor
//----------------------------------------------------------------------------
// * fps : number of times step() will be called per second of elapsed time
//----------------------------------------------------------------------------
AnimatedObject::AnimatedObject(float fps) :
    fps_(fps > 0 ? fps : FPS),
    clock_(0),
    frozen_(false)
{}

//----------------------------------------------------------------------------
// - Get Local Frames/Second
//----------------------------------------------------------------------------
float AnimatedObject::getFPS() const
{
    return fps_;
}

//----------------------------------------------------------------------------
// - Set Local Frames/Second
//----------------------------------------------------------------------------
// * fps : calls to step() per second of elapsed time
//----------------------------------------------------------------------------
void AnimatedObject::setFPS(float fps)
{
    // An fps of zero or less is undefined
    if(fps > 0)
    {
        fps_ = fps;
    }
}

//----------------------------------------------------------------------------
// - Is Object Frozen?
//----------------------------------------------------------------------------
bool AnimatedObject::frozen() const
{
    return frozen_;
}

//----------------------------------------------------------------------------
// - Set Frozen Status
//----------------------------------------------------------------------------
// * frozen : sets a flag indicating time is frozen for this object
//----------------------------------------------------------------------------
void AnimatedObject::setFrozen(bool frozen)
{
    frozen_ = frozen;
}

//----------------------------------------------------------------------------
// - Set Frozen to True
//----------------------------------------------------------------------------
void AnimatedObject::freeze()
{
    frozen_ = true;
}

//----------------------------------------------------------------------------
// - Set Frozen to False
//----------------------------------------------------------------------------
void AnimatedObject::unfreeze()
{
    frozen_ = false;
}

//----------------------------------------------------------------------------
// - Update
//----------------------------------------------------------------------------
// * elapsed : relative time passed since last update
//----------------------------------------------------------------------------
void AnimatedObject::update(float elapsed)
{
    if(!frozen_)
    {
        clock_ += elapsed * fps_;
        while(clock_ >= 1)
        {
            step();
            clock_ -= 1;
        }
    }
}