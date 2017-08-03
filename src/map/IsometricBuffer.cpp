#include "IsometricBuffer.h"
#include "../settings.h"
#include <algorithm>

//----------------------------------------------------------------------------
// - Isometric Buffer Constructor
//----------------------------------------------------------------------------
// * scale : (x,y,z) pixels per local unit scaling vector
//----------------------------------------------------------------------------
IsometricBuffer::IsometricBuffer() :
    AnimatedObject(FPS),
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
// - Add Object to Buffer
//----------------------------------------------------------------------------
// * obj : new isometric object to add to the depth buffer
//----------------------------------------------------------------------------  
void IsometricBuffer::add(const IsometricObject* obj)
{
    IsometricNode* node = new IsometricNode(const_cast<IsometricObject*>(obj), this);
    objects_.push_back(node);
    alert();
}

//----------------------------------------------------------------------------
// - Remove Object from Buffer
//----------------------------------------------------------------------------
// * obj : isometric object to remove from the depth buffer
//----------------------------------------------------------------------------  
void IsometricBuffer::remove(const IsometricObject* obj)
{
    remove(obj->getHandler());
    delete obj->getHandler(); 
}

//----------------------------------------------------------------------------
// - Remove Node from Buffer
//----------------------------------------------------------------------------
// * node : node to clean up before destruction
//----------------------------------------------------------------------------  
void IsometricBuffer::remove(IsometricNode* node)
{
    // Remove node's target object from sorted images
    auto sortedTarget = std::find(sorted_.begin(), sorted_.end(), node->target());
    if(sortedTarget != sorted_.end())
    {
        sorted_.erase(sortedTarget);
    }

    for(auto node_it = objects_.begin(); node_it != objects_.end();)
    {
        if(*node_it != node)
        {
            // Remove any incoming edges for the extinct node
            (*node_it++)->removeChild(node);
        }
        else
        {
            // Remove the node itself
            node_it = objects_.erase(node_it);
        }
    }  
}

//----------------------------------------------------------------------------
// - Alert Buffer of Status Change
//----------------------------------------------------------------------------
void IsometricBuffer::alert()
{
    dirty_ = true;
}

//----------------------------------------------------------------------------
// - Isometrical Sort (Full)
//----------------------------------------------------------------------------  
// Sorts ALL isometric objects into drawing order using a topological sort
// on a bounding rectangle intersection graph, using directed edges defined by
// an isometric view ordering
//----------------------------------------------------------------------------
void IsometricBuffer::sort()
{    
    // Remove any pre-existing edges for all nodes
    for(int n = 0; n < objects_.size(); n++)
    {
        objects_[n]->detach();
    }
    
    // Connect nodes with intersecting bounding boxes using directed edges
    for(int i = 0; i < objects_.size(); i++)
    {        

        for(int j = i + 1; j < objects_.size(); j++)
        {
            // If bounding boxes intersect, establish a directed connection (parent-child)
            if(objects_[i]->getBounds().intersects(objects_[j]->getBounds())){
                objects_[i]->attach(objects_[j]);                
            }
        }

        // Clears "dirty" flag for this node during next sort
        objects_[i]->resolve();
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
void IsometricBuffer::partialSort(const std::vector<IsometricNode*>& dirty_nodes)
{
    for(int d = 0; d < dirty_nodes.size(); d++)
    {
        // Remove any pre-existing outgoing edges
        dirty_nodes[d]->detach();

        // Remove any incoming edges
        for(int parent = 0; parent < objects_.size(); parent++)
        {
            objects_[parent]->removeChild(dirty_nodes[d]);
        }
    }

    // Reconnect all edges to dirty nodes
    for(int d = 0; d < dirty_nodes.size(); d++)
    {
        // Check all possible other nodes for connection
        for(int neighbor = 0; neighbor < objects_.size(); neighbor++)
        {
            // No edges to self
            if(dirty_nodes[d] != objects_[neighbor])
            {
                // If bounding boxes intersect, draw an edge
                if(dirty_nodes[d]->getBounds().intersects(objects_[neighbor]->getBounds()))
                {
                    dirty_nodes[d]->attach(objects_[neighbor]);
                }
            }
        }

        // Clears "dirty" flag for this node during next sort        
        dirty_nodes[d]->resolve();
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
    // Clear all visited flags
    for(int n = 0; n < objects_.size(); n++)
    {
        objects_[n]->setVisited(false);
    }

    // Sort each non-visited node
    for(int n = 0; n < objects_.size(); n++)
    {
        if(!objects_[n]->visited())
        {
            topologicalTraverse(objects_[n]);            
        }
    }
}

//----------------------------------------------------------------------------
// - Topological Traverse (Recursive)
//---------------------------------------------------------------------------- 
// * node : current node being recursively traversed for sorting
// * visited : set which tracks all recursively visited nodes
//---------------------------------------------------------------------------- 
void IsometricBuffer::topologicalTraverse(IsometricNode* node)
{
    // Visit this node
    node->setVisited(true);

    // Recur for each non-visited child
    for(auto child : node->children())
    {
        if(!child->visited())
        {
            topologicalTraverse(child);            
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
    for(int n = 0; n < sorted_.size(); n++)
    {
        sf::RenderStates state = states;
		state.transform.translate(sorted_[n]->getGlobalPosition());

		target.draw(*sorted_[n], state);
    }
}
#include <iostream>
//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Calls the isometric sort event if any nodes require re-sorting
//----------------------------------------------------------------------------
void IsometricBuffer::step()
{
    if(dirty_){
        std::vector<IsometricNode*> dirty;

        for(int n = 0; n < objects_.size(); n++)
        {
            if(objects_[n]->dirty())
            {
                dirty.push_back(objects_[n]);
            }
        }

        // If more than half of the nodes are dirty, execute a full sort
        
        if(dirty.size() > objects_.size() / 2)
        {
            sort();
        }
        // Otherwise, execute a partial sort only on the dirty nodes
        else
        {
            std::cout << "partially sorting " << dirty.size() << " objects ... " << std::endl;
            partialSort(dirty);
        }
    }       
}