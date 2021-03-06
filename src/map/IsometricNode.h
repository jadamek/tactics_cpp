#ifndef TACTICS_ISOMETRIC_NODE_H
#define TACTICS_ISOMETRIC_NODE_H

#include "../objects/IsometricObject.h"
#include <deque>

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
    void                            resolve();
    void                            attach(IsometricNode* node);
    void                            detach();
    void                            removeChild(IsometricNode* child);
    const std::deque<IsometricNode*>& children() const;
    bool                            dirty() const;
    const sf::FloatRect&            getBounds() const;
    void                            setBounds(const sf::FloatRect& bounds);
    void                            deactivate();
    int                             visited() const;
    void                            setVisited(bool visited);
    
private:
    bool                            compare(const IsometricObject* a, const IsometricObject* b) const;

// Members
    IsometricObject*                target_;
    IsometricBuffer*                container_;
    bool                            dirty_;
    std::deque<IsometricNode*>      children_;
    sf::FloatRect                   bounds_;
    bool                            visited_;
};

#endif