#include "TargetSelector.h"
#include <math.h>
/*
//----------------------------------------------------------------------------
// - Target Selector Constructor
//----------------------------------------------------------------------------
// * area : set of positions that describe the area of interest
// * areaTexture : bitmap to use for the sprites to be drawn at position of
//      interest
// * areaColor : color of the area of interest
// * cursorTexture : bitmap to use for the cursor itself
// * map : map on which the area exists
// * action : action to execute when a valid target is selected
//----------------------------------------------------------------------------
TargetSelector::TargetSelector(const std::vector<sf::Vector2i>& area, const sf::Texture& areaTexture,
    const sf::Color& areaColor, const sf::Texture& cursorTexture, Map* map, std::function<void()> action) :
    Cursor(cursorTexture, map, action),
    area_(area.size(), SpriteArea(areaTexture, areaColor))
{
    if(map)
    {
        for(int i = 0; i < area_.size(); i++)
        {
            area_[i].setPosition(sf::Vector3f(area[i].x, area[i].y, map->height(area[i].x, area[i].y)));
            map->addObject(&area_[i]);
        }
    }    
}

//----------------------------------------------------------------------------
// - Target Selector Destructor
//----------------------------------------------------------------------------
TargetSelector::~TargetSelector()
{}

//----------------------------------------------------------------------------
// - Area Contains Point?
//----------------------------------------------------------------------------
// * position : X-Y coordinate which is being compared to all positions in the
//      area of interest
//----------------------------------------------------------------------------
bool TargetSelector::contains(const sf::Vector2f& position) const
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
*/