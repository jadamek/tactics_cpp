#include "Cursor.h"

//----------------------------------------------------------------------------
// - Cursor Constructor
//----------------------------------------------------------------------------
// * texture : bitmap the cursor uses as its sprite display
// * map : map this cursor belongs to and moves within
// * parent : handler designated as input controller above this
//----------------------------------------------------------------------------
Cursor::Cursor(const sf::Texture& texture, Map* map, InputHandler* parent) :
    map_(map),
    sprite_(texture),
    hidden_(false)
{
    sprite_.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

    if(map_)
    {
        setPosition(sf::Vector3f(0, 0, std::max(0.f, map_->height(0, 0))));
    }    
}

//----------------------------------------------------------------------------
// - Cursor Destructor
//----------------------------------------------------------------------------
Cursor::~Cursor()
{}

//----------------------------------------------------------------------------
// - Go To Position
//----------------------------------------------------------------------------
// * position : (x,y) position on the map to move to
//----------------------------------------------------------------------------
void Cursor::goTo(const sf::Vector2f& position)
{
    if(map_)
    {
        if(map_->at(position.x, position.y))
        {
            moveTo(sf::Vector3f(position.x, position.y, map_->height(position.x, position.y)));
        }
    }
}

//----------------------------------------------------------------------------
// - Select Player
//----------------------------------------------------------------------------
// Returns the player located at the current position, if one is present
//----------------------------------------------------------------------------
Actor* Cursor::select() const
{
    if(map_)
    {
        return map_->playerAt(position().x, position().y);
    }
    else
    {
        return 0;
    }
}

//----------------------------------------------------------------------------
// - Get Map Environment
//----------------------------------------------------------------------------
// Returns the tile located at the current position, or the map object located
// at the tile if one is present
//----------------------------------------------------------------------------
MapObject* Cursor::getEnvironment() const
{
    if(map_)
    {
        Tile* tile = 0;
        if((tile = map_->at(position().x, position().y)))
        {
            if(tile->getOccupant()){
                return tile->getOccupant();
            }
            else
            {
                return tile;
            }
        }
    }

    return 0;
}

//----------------------------------------------------------------------------
// - Get Height (Override)
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the object
//----------------------------------------------------------------------------
float Cursor::getHeight(const sf::Vector2f& position) const
{
    return 0;
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this tile's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect Cursor::getGlobalBounds() const
{
    return sprite_.getGlobalBounds();
}

#include <iostream>

//----------------------------------------------------------------------------
// Poll Input Event (Override)
//----------------------------------------------------------------------------
void Cursor::poll()
{
    if(!busy_ && !hidden_){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            std::cout << "Hey." << std::endl;
        }
    }    
}

//----------------------------------------------------------------------------
// Show Cursor (Override)
//----------------------------------------------------------------------------
void Cursor::show()
{
    hidden_ = false;
}

//----------------------------------------------------------------------------
// Hide Cursor (Override)
//----------------------------------------------------------------------------
void Cursor::hide()
{
    hidden_ = true;
}

//----------------------------------------------------------------------------
// Cursor Busy? (Override)
//----------------------------------------------------------------------------
bool Cursor::busy() const
{
    return busy_ || moving();
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(!hidden_){
        target.draw(sprite_, states);        
    }
}