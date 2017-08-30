#ifndef TACTICS_INPUT_HANDLER_H
#define TACTICS_INPUT_HANDLER_H

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
    virtual bool    active() const;
    void            setActive(bool active);    
    virtual bool    busy() const;
    void            setBusy(bool busy);
    
// Members
protected:
    bool            busy_;
    bool            active_;
};

#endif