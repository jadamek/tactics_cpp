#ifndef TACTICS_ISOMETRIC_NODE_H
#define TACTICS_ISOMETRIC_NODE_H

#include "../objects/IsometricObject.h"

//================================================================================
// ** IsometricNode
//================================================================================
// Represents a single node in the isometric ordering container
//================================================================================
class IsometricNode
{
public:
    IsometricNode(IsometricObject* target);
    ~IsometricNode();
    
    const IsometricObject*  target() const;
    IsometricObject*        target();
    void alert();

private:
    IsometricObject*    target_;
    bool                dirty_;
};

#endif