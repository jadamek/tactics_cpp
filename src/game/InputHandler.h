#ifndef TACTICS_INPUT_HANDLER_H
#define TACTICS_INPUT_HANDLER_H

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
    InputHandler();
    virtual ~InputHandler();

    virtual void    poll() = 0;
    virtual void    hide() = 0;
    virtual void    show() = 0;
    virtual bool    busy() const;
    void            setBusy(bool busy);
    
// Members
protected:
    bool            busy_;
};

#endif