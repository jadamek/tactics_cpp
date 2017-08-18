#include "InputHandler.h"

//----------------------------------------------------------------------------
// - InputHandler Constructor
//----------------------------------------------------------------------------
// * parent : handler designated as input controller above this
//----------------------------------------------------------------------------
InputHandler::InputHandler(InputHandler* parent) :
    parent_(parent)
{}

//----------------------------------------------------------------------------
// - InputHandler Constructor
//----------------------------------------------------------------------------
InputHandler::~InputHandler()
{}