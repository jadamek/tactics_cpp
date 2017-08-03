#ifndef TACTICS_ISOMETRIC_BUFFER_H
#define TACTICS_ISOMETRIC_BUFFER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "IsometricNode.h"
#include "../objects/AnimatedObject.h"

//================================================================================
// ** IsometricBuffer
//================================================================================
// Represents a depth buffer of drawable isometric objects
//================================================================================
class IsometricBuffer : public sf::Drawable, public AnimatedObject
{
// Methods
public:
    IsometricBuffer();    
    ~IsometricBuffer();

    void                add(const IsometricObject* obj);
    void                remove(const IsometricObject* obj);
    void                remove(IsometricNode* node);
    void                alert();
    void                sort();

private:
    void                partialSort(const std::vector<IsometricNode*>& dirty_nodes);
    void                topologicalSort();
    void                topologicalTraverse(IsometricNode* node);
    void                draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void                step();

// Members
    std::vector<IsometricNode*>         objects_;
    std::vector<const IsometricObject*> sorted_;
    bool                                dirty_;
};

#endif