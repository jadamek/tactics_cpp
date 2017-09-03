#include "InputManager.h"
#include "../settings.h"
#include <math.h>

//----------------------------------------------------------------------------
// - Input Manager Constructor (private)
//----------------------------------------------------------------------------
InputManager::InputManager() :
    AnimatedObject(FPS / 6),
    delay_(0),
    throttle_(ceil(15 * getFPS() / FPS))
{}

//----------------------------------------------------------------------------
// - Input Manager Constructor (private, empty)
//----------------------------------------------------------------------------
InputManager::InputManager(const InputManager&) :
    throttle_(0)
{}

//----------------------------------------------------------------------------
// - Input Manager Destructor
//----------------------------------------------------------------------------
InputManager::~InputManager()
{}

//----------------------------------------------------------------------------
// - Get Single Instance
//----------------------------------------------------------------------------
InputManager& InputManager::instance()
{
    static InputManager instance;
    return instance;
}

//----------------------------------------------------------------------------
// - Push Handler
//----------------------------------------------------------------------------
// * handler : new input handling object to push onto the handler stack
//----------------------------------------------------------------------------
void InputManager::push(InputHandler* handler)
{
    delay_ = throttle_;
    
    if(!handlerStack_.empty())
    {
        handlerStack_.top()->setActive(false);           
    }
    handlerStack_.push(handler);
    handlerStack_.top()->setActive(true);
}

//----------------------------------------------------------------------------
// - Pop Handler
//----------------------------------------------------------------------------
void InputManager::pop()
{
    if(!handlerStack_.empty())
    {
        delay_ = throttle_;

        handlerStack_.top()->setActive(false);
        handlerStack_.pop();
        handlerStack_.top()->setActive(true);    
    }
}

//----------------------------------------------------------------------------
// - Pop All Before
//----------------------------------------------------------------------------
// handler : input controller to become the new top after popping all higher
//      controllers
//----------------------------------------------------------------------------
void InputManager::popTo(InputHandler* handler)
{
    pop();
    while(!handlerStack_.empty())
    {
        if(handlerStack_.top() != handler)
        {
            pop();
        }
        else break;
    }
}

//----------------------------------------------------------------------------
// - Clear Handler Stack
//----------------------------------------------------------------------------
void InputManager::clear()
{
    delay_ = throttle_;
    
    while(!handlerStack_.empty())
    {
        handlerStack_.top()->setActive(false);
        handlerStack_.pop();
    }
}

//----------------------------------------------------------------------------
// - Get Top Handler
//----------------------------------------------------------------------------
InputHandler* InputManager::getHandler() const
{
    if(!handlerStack_.empty()){
        return handlerStack_.top();    
    }
    else
    {
        return 0;
    }
}

//----------------------------------------------------------------------------
// - Poll Current Handler
//----------------------------------------------------------------------------
// Signals the top handler to poll for input events
//----------------------------------------------------------------------------
void InputManager::poll()
{
    if(!handlerStack_.empty() && delay_ <= 0)
    {
        if(!handlerStack_.top()->busy() && handlerStack_.top()->active())
        {
            handlerStack_.top()->poll();
        }
    }
}

//----------------------------------------------------------------------------
// - Increment Frame (Override)
//----------------------------------------------------------------------------
void InputManager::step()
{
    poll();

    if(delay_ > 0) delay_--;
}