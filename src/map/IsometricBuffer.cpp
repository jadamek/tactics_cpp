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
    scale_(sf::Vector3f(std::max(1.f, scale.x), std::max(1.f, scale.y), std::max(1.f, scale.z)))
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
}

//----------------------------------------------------------------------------
// - Remove Object from Buffer
//----------------------------------------------------------------------------
// * obj : isometric object to remove from the depth buffer
//----------------------------------------------------------------------------  
void IsometricBuffer::remove(const IsometricObject* obj)
{
    objects_.erase(obj->getHandler());
}
#include <iostream>

//----------------------------------------------------------------------------
// - Isometrical Sort
//----------------------------------------------------------------------------  
// Sorts all isometric objects into drawing order using a topological sort
// on a bounding rectangle intersection graph, using directed edges defined by
// an isometric view ordering
//----------------------------------------------------------------------------
void IsometricBuffer::isometricSort()
{
    std::cout << "Sorting :" << std::endl;
    sorted_.clear();
    sinks_.clear();

    // Connect nodes with intersecting bounding boxes using directed edges
    for(auto node_it = objects_.begin(); node_it != objects_.end(); node_it++)
    {
        auto neighbor_it = node_it;

        // Compute isometric bounding box for this node
        sf::FloatRect node_bounds = (*node_it)->target()->getGlobalBounds();
        sf::Vector2f isometric_position = localToIso((*node_it)->target()->position());
        node_bounds.left += isometric_position.x;
        node_bounds.top += isometric_position.y;

        /*
        std::cout << "   " << (*node_it) << " (" << (*node_it)->target()->position().x << ", " << (*node_it)->target()->position().y << ", "
            << (*node_it)->target()->position().z << ", " << ") x " << (*node_it)->target()->getHeight()
            << " [" << node_bounds.left << ", " << node_bounds.top << ", "
            << node_bounds.width << ", " << node_bounds.height << "]" << std::endl;
        */

        for(++neighbor_it; neighbor_it != objects_.end(); neighbor_it++)
        {
            // Compute isometric bounding box for potential neighbor
            sf::FloatRect neighbor_bounds = (*neighbor_it)->target()->getGlobalBounds();
            isometric_position = localToIso((*neighbor_it)->target()->position());
            neighbor_bounds.left += isometric_position.x;
            neighbor_bounds.top += isometric_position.y;

            // If bounding boxes intersect, establish a directed connection (parent-child)
            if(node_bounds.intersects(neighbor_bounds)){
                (*node_it)->attach(*neighbor_it);                
            }
        }
        
        /*
        for(auto parent : (*node_it)->parents())
        {
            std::cout << "     < " << parent << std::endl;
        }

        for(auto child : (*node_it)->children())
        {
            std::cout << "     > " << child << std::endl;
        }
        */

        // Will decide if this is sink or source later >_> ...
        if((*node_it)->children().empty())
        {
            sinks_.insert(*node_it);
        }
    }

    std::cout << std::endl << "Sinks: " << std::endl;
    for(auto sink : sinks_)
    {
        std::cout << " " << sink << std::endl;
    }
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------  
// Draw all isometric objects in the buffer in their proper isometric position
//----------------------------------------------------------------------------
void IsometricBuffer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(auto node : objects_)
    {
        sf::RenderStates state = states;
		state.transform.translate(localToIso(node->target()->position()));

		target.draw(*node->target(), state);
    }
}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Calls the isometric sort event if any nodes require re-sorting
//----------------------------------------------------------------------------
void IsometricBuffer::step()
{
    for(IsometricNode* node : objects_)
    {
        if(node->dirty())
        {
            isometricSort();        
            break;
        }
    }
}