#include "SpriteArea.h"
#include <math.h>

//----------------------------------------------------------------------------
// - Sprite Area Constructor
//----------------------------------------------------------------------------
// * texture : bitmap to use for the sprites to be drawn at position of
//      interest
// * area : set of positions that describe the area of interest
// * map : height map used to determine each positions height
// * color : color of the area of interest
//----------------------------------------------------------------------------
SpriteArea::SpriteArea(const sf::Texture& texture, const std::vector<sf::Vector2f>& area, Map* map, const sf::Color& color) :
    area_(area.size(), SpriteAreaSquare(texture, color))
{
    if(map)
    {
        for(int i = 0; i < area_.size(); i++)
        {
            area_[i].setPosition(sf::Vector3f(area[i].x, area[i].y, map->height(area[i].x, area[i].y)));
        }
    }    
}

//----------------------------------------------------------------------------
// - Sprite Area Destructor
//----------------------------------------------------------------------------
SpriteArea::~SpriteArea()
{

}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this actor's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect SpriteArea::getGlobalBounds() const
{
    return sf::FloatRect();
}

//----------------------------------------------------------------------------
// - Join Buffer (Override)
//----------------------------------------------------------------------------
// * buffer : isometric container to add all area squares
//----------------------------------------------------------------------------
void SpriteArea::join(IsometricBuffer* buffer) const
{
    for(int i = 0; i < area_.size(); i++)
    {
        buffer->insert(&area_[i]);
    }
}

//----------------------------------------------------------------------------
// - Area Contains Point?
//----------------------------------------------------------------------------
// * position : X-Y coordinate which is being compared to all positions in the
//      area of interest
//----------------------------------------------------------------------------
bool SpriteArea::contains(const sf::Vector2f& position) const
{
    int x = round(position.x);
    int y = round(position.y);
    
    for(int i = 0; i < area_.size(); i++)
    {
        if(area_[i].position().x == x && area_[i].position().y == y)
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void SpriteArea::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(int i = 0; i < area_.size(); area_)
    {
        target.draw(area_[i], states);        
    }
}