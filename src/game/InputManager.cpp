#include "InputManager.h"

//----------------------------------------------------------------------------
// - Input Manager Constructor (private)
//----------------------------------------------------------------------------
InputManager::InputManager()
{}

//----------------------------------------------------------------------------
// - Input Manager Constructor (private, empty)
//----------------------------------------------------------------------------
InputManager::InputManager(const InputManager&)
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
// - Poll Current Handler
//----------------------------------------------------------------------------
// Signals the top handler to poll for input events
//----------------------------------------------------------------------------
void InputManager::poll()
{
    if(!handlerStack_.empty())
    {
        if(!handlerStack_.top()->busy() && handlerStack_.top()->active())
        {
            handlerStack_.top()->poll();
        }
    }
}

//----------------------------------------------------------------------------
// - Push Handler
//----------------------------------------------------------------------------
// * handler : new input handling object to push onto the handler stack
//----------------------------------------------------------------------------
void InputManager::push(InputHandler* handler)
{
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
        handlerStack_.top()->setActive(false);
        handlerStack_.pop();
        handlerStack_.top()->setActive(true);    
    }
}

//----------------------------------------------------------------------------
// - Pop All Before
//----------------------------------------------------------------------------
// handler : input controller that  all higher in the stack should be popped
//      up until
//----------------------------------------------------------------------------
void InputManager::popTo(InputHandler* handler)
{
    while(handlerStack_.top() != handler)
    {
        handlerStack_.top()->setActive(false);
        handlerStack_.pop();
    }
    handlerStack_.top()->setActive(true);
}

//----------------------------------------------------------------------------
// - Clear Handler Stack
//----------------------------------------------------------------------------
void InputManager::clear()
{
    while(!handlerStack_.empty())
    {
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