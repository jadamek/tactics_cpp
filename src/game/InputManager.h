#ifndef TACTICS_INPUT_MANAGER_H
#define TACTICS_INPUT_MANAGER_H

#include "InputHandler.h"
#include <stack>

//================================================================================
// ** InputManager
//================================================================================
// Singleton stack of input handling objects. Hides lower handlers on push,
// revealing them again when the higher handler is popped
//================================================================================
class InputManager
{
// Methods
private:
    InputManager();
    InputManager(const InputManager&);

public:
    ~InputManager();

    static InputManager&    instance();
    void                    poll();
    void                    push(InputHandler* handler);
    void                    pop();
    void                    popTo(InputHandler* handler);
    void                    clear();
    InputHandler*           getHandler() const;

// Members
private:
    std::stack<InputHandler*>   handlerStack_;
};

#endif