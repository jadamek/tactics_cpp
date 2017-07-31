#include "IsometricBuffer.h"
#include "../settings.h"
#include <algorithm>

//----------------------------------------------------------------------------
// - Isometric Buffer Constructor
//----------------------------------------------------------------------------
// * scale : (x,y,z) pixels per local unit scaling vector
//----------------------------------------------------------------------------
IsometricBuffer::IsometricBuffer(const sf::Vector3f& scale) :
    AnimatedObject(1.f),
    scale_(sf::Vector3f(std::max(1.f, scale.x), std::max(1.f, scale.y), std::max(1.f, scale.z))),
    dirty_(false)
{}

//----------------------------------------------------------------------------
// - Isometric Buffer Destructor
//----------------------------------------------------------------------------  
IsometricBuffer::~IsometricBuffer()
{
    for(auto node : objects_)
    {
        delete node;
    }
}

//----------------------------------------------------------------------------
// - Get Isometric Scaling Vector
//----------------------------------------------------------------------------
const sf::Vector3f& IsometricBuffer::getIsometricScale() const
{
    return scale_;
}

//----------------------------------------------------------------------------
// - Isometric Buffer Destructor
//----------------------------------------------------------------------------
// Converts a 3-Dimensional local coordinate position to a 2-Dimensional
// isometric pixel coordinate position
//----------------------------------------------------------------------------
sf::Vector2f IsometricBuffer::localToIso(const sf::Vector3f& position) const
{
    float x = 0.5 * scale_.x * (position.x - position.y);
    float y = 0.5 * scale_.y * (position.x + position.y) - scale_.z * position.z;

    return sf::Vector2f(x, y);
}

//----------------------------------------------------------------------------
// - Add Object to Buffer
//----------------------------------------------------------------------------
// * obj : new isometric object to add to the depth buffer
//----------------------------------------------------------------------------  
void IsometricBuffer::add(const IsometricObject* obj)
{
    IsometricNode* node = new IsometricNode(const_cast<IsometricObject*>(obj), this);
    objects_.insert(node);
    alert();
}

//----------------------------------------------------------------------------
// - Remove Object from Buffer
//----------------------------------------------------------------------------
// * obj : isometric object to remove from the depth buffer
//----------------------------------------------------------------------------  
void IsometricBuffer::remove(const IsometricObject* obj)
{
    IsometricNode* node = obj->getHandler();

    // Remove any incoming edges for the extinct node
    for(auto neighbor : objects_)
    {
        neighbor->removeChild(node);
    }

    // Remove object's node
    objects_.erase(node);

    // Remove object from sorted images
    auto toRemove = std::find(sorted_.begin(), sorted_.end(), obj);
    if(toRemove != sorted_.end())
    {
        sorted_.erase(toRemove);
    }
}

//----------------------------------------------------------------------------
// - Alert Buffer of Status Change
//----------------------------------------------------------------------------
void IsometricBuffer::alert()
{
    dirty_ = true;
}

#include <iostream>

//----------------------------------------------------------------------------
// - Isometrical Sort (Full)
//----------------------------------------------------------------------------  
// Sorts ALL isometric objects into drawing order using a topological sort
// on a bounding rectangle intersection graph, using directed edges defined by
// an isometric view ordering
//----------------------------------------------------------------------------
void IsometricBuffer::isometricSort()
{    
    // Remove any pre-existing edges for all nodes
    for(auto node : objects_)
    {
        node->detach();
    }

    // Compute bounding box for every node
    for(auto node : objects_)
    {
        sf::FloatRect node_bounds = node->target()->getGlobalBounds();
        sf::Vector2f isometric_position = localToIso(node->target()->position());
        node_bounds.left += isometric_position.x;
        node_bounds.top += isometric_position.y;

        node->setBounds(node_bounds);
    }
    
    // Connect nodes with intersecting bounding boxes using directed edges
    for(auto node_it = objects_.begin(); node_it != objects_.end(); node_it++)
    {
        auto neighbor_it = node_it;

        for(++neighbor_it; neighbor_it != objects_.end(); neighbor_it++)
        {
            // If bounding boxes intersect, establish a directed connection (parent-child)
            if((*node_it)->getBounds().intersects((*neighbor_it)->getBounds())){
                (*node_it)->attach(*neighbor_it);                
            }
        }

        // Clears "dirty" flag for this node during next sort
        (*node_it)->resolve();
    }

    for(auto node : objects_)
    {
        if(node->dirty())
        {
            std::cout << "Dirt??" << std::endl;
        }
    }

    // Clear any previous sorting
    sorted_.clear();

    // Finally, topologically sort all nodes
    topologicalSort();

    // Clear buffer-wide dirty flag
    dirty_ = false;
}

//----------------------------------------------------------------------------
// - Isometrical Sort (Full)
//----------------------------------------------------------------------------
// * dirty_nodes : set of all nodes which have updated their positions
// Re-sorts isometric nodes into drawing order using a topological sort,
// limiting re-attachment and bounding box computation to dirty nodes
//----------------------------------------------------------------------------
void IsometricBuffer::partialIsometricSort(const std::set<IsometricNode*>& dirty_nodes)
{
    for(auto node : dirty_nodes)
    {
        // Remove any pre-existing outgoing edges
        node->detach();

        // Remove any incoming edges
        for(auto parent : objects_)
        {
            parent->removeChild(node);
        }

        // Re-compute bounding box        
        sf::FloatRect node_bounds = node->target()->getGlobalBounds();
        sf::Vector2f isometric_position = localToIso(node->target()->position());
        node_bounds.left += isometric_position.x;
        node_bounds.top += isometric_position.y;

        node->setBounds(node_bounds);
    }

    // Reconnect all edges to dirty nodes
    for(auto node : dirty_nodes)
    {
        // Check all possible other nodes for connection
        for(auto neighbor : objects_)
        {
            // No edges to self
            if(node != neighbor)
            {
                // If bounding boxes intersect, draw an edge
                if(node->getBounds().intersects(neighbor->getBounds()))
                {
                    node->attach(neighbor);
                }
            }
        }

        // Clears "dirty" flag for this node during next sort        
        node->resolve();
    }

    // Clear any previous sorting
    sorted_.clear();
    
    // Finally, topologically sort all nodes
    topologicalSort();

    // Clear buffer-wide dirty flag
    dirty_ = false;
}

//----------------------------------------------------------------------------
// - Topological Sort
//---------------------------------------------------------------------------- 
// Uses a recursive DPS traversal to sort objects topologically through their
// child edges
//---------------------------------------------------------------------------- 
void IsometricBuffer::topologicalSort()
{
    std::set<IsometricNode*> visited;
    for(auto node : objects_)
    {
        if(visited.count(node) == 0)
        {
            topologicalTraverse(node, visited);            
        }
    }
}

//----------------------------------------------------------------------------
// - Topological Traverse (Recursive)
//---------------------------------------------------------------------------- 
// * node : current node being recursively traversed for sorting
// * visited : set which tracks all recursively visited nodes
//---------------------------------------------------------------------------- 
void IsometricBuffer::topologicalTraverse(IsometricNode* node, std::set<IsometricNode*>& visited)
{
    // Visit this node
    visited.insert(node);

    // Recur for each non-visited child
    for(auto child : node->children())
    {
        if(visited.count(child) == 0)
        {
            topologicalTraverse(child, visited);            
        }
    }

    // Push current node to the sorted drawing queue
    sorted_.push_back(node->target());
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------  
// Draw all isometric objects in the buffer in their proper isometric position
//----------------------------------------------------------------------------
void IsometricBuffer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(auto obj : sorted_)
    {
        sf::RenderStates state = states;
		state.transform.translate(localToIso(obj->position()));

		target.draw(*obj, state);
    }
}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Calls the isometric sort event if any nodes require re-sorting
//----------------------------------------------------------------------------
void IsometricBuffer::step()
{
    if(dirty_){
        std::set<IsometricNode*> dirty;

        for(IsometricNode* node : objects_)
        {
            if(node->dirty())
            {
                dirty.insert(node);
            }
        }

        // If more than half of the nodes are dirty, execute a full sort
        
        if(dirty.size() > objects_.size() / 2)
        {
            isometricSort();
        }
        // Otherwise, execute a partial sort only on the dirty nodes
        else
        {
            partialIsometricSort(dirty);
        }
    }       
}