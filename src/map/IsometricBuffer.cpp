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
    IsometricNode* node = new IsometricNode(const_cast<IsometricObject*>(obj));
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

//----------------------------------------------------------------------------
// - Isometrical Sort
//----------------------------------------------------------------------------  
// Sorts all isometric objects into drawing order using a topological sort
// on a bounding rectangle intersection graph, using directed edges defined by
// an isometric view ordering
//----------------------------------------------------------------------------
void IsometricBuffer::isometricSort()
{

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
#include <iostream>
//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Calls the isometric sort event if any nodes require re-sorting
//----------------------------------------------------------------------------
void IsometricBuffer::step()
{
    std::cout << "Sorting :" << std::endl;
}