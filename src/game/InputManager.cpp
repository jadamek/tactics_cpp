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
        if(!handlerStack_.top()->busy())
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
        handlerStack_.top()->hide();           
    }
    handlerStack_.push(handler);
    handlerStack_.top()->show(); 
}

//----------------------------------------------------------------------------
// - Pop Handler
//----------------------------------------------------------------------------
void InputManager::pop()
{
    if(!handlerStack_.empty())
    {
        handlerStack_.top()->hide();
        handlerStack_.pop();
        handlerStack_.top()->show();    
    }
}

//----------------------------------------------------------------------------
// - Pop All But Last Handler
//----------------------------------------------------------------------------
void InputManager::popToLast()
{
    while(handlerStack_.size() > 1)
    {
        handlerStack_.top()->hide();
        handlerStack_.pop();
    }
    handlerStack_.top()->show();
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