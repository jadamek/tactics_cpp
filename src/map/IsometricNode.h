#ifndef TACTICS_ISOMETRIC_NODE_H
#define TACTICS_ISOMETRIC_NODE_H

#include "../objects/IsometricObject.h"
#include <set>

//================================================================================
// ** IsometricNode
//================================================================================
// Represents a single node in the isometric ordering container
//================================================================================
class IsometricNode
{
// Methods
public:
    IsometricNode(IsometricObject* target, IsometricBuffer* container);
    ~IsometricNode();
    
    const IsometricObject*          target() const;
    IsometricObject*                target();
    void                            alert();
    void                            attach(IsometricNode* node);
    void                            detach();
    const std::set<IsometricNode*>& children() const;
    bool                            dirty() const;

private:
    bool                            compare(const IsometricObject* a, const IsometricObject* b) const;

// Members
    IsometricObject*                target_;
    IsometricBuffer*                container_;
    bool                            dirty_;
    std::set<IsometricNode*>        children_;
};

#endif