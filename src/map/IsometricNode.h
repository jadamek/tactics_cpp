#ifndef TACTICS_ISOMETRIC_BUFFER_H
#define TACTICS_ISOMETRIC_BUFFER_H

#include "../objects/IsometricObject.h"

//================================================================================
// ** IsometricNode
//================================================================================
// Represents a single node in the isometric ordering container
//================================================================================
class IsometricNode
{
public:
    IsometricNode();
    ~IsometricNode();
    
    void update();

private:
    bool dirty_;
};

#endif