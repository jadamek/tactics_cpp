#ifndef TACTICS_INPUT_MANAGER_H
#define TACTICS_INPUT_MANAGER_H

#include "../control/InputHandler.h"
#include "../objects/AnimatedObject.h"
#include <stack>

//================================================================================
// ** InputManager
//================================================================================
// Singleton stack of input handling objects. Hides lower handlers on push,
// revealing them again when the higher handler is popped
//================================================================================
class InputManager : public AnimatedObject
{
// Methods
private:
    InputManager();
    InputManager(const InputManager&);

public:
    ~InputManager();

    static InputManager&    instance();
    void                    push(InputHandler* handler);
    void                    pop();
    void                    popTo(InputHandler* handler);
    void                    clear();
    InputHandler*           getHandler() const;

protected:
    void                    poll();
    virtual void            step();

// Members
private:
    std::stack<InputHandler*>   handlerStack_;
    int                     delay_;
    const int               throttle_;
};

#endif