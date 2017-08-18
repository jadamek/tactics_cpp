#ifndef TACTICS_INPUT_HANDLER
#define TACTICS_INPUT_HANDLER

#include <SFML/Graphics.hpp>

//================================================================================
// ** InputHandler
//================================================================================
// Abstract base for all objects able to handle input events such as keystrokes,
// touchpad or mouse events
//================================================================================
class InputHandler
{
// Methods
public:
    InputHandler(InputHandler* parent);
    virtual ~InputHandler();

    virtual void    pollInput() = 0;
    
// Members
protected:
    InputHandler*   parent_;
};

#endif