#include "ManhattanArea.h"
#include <algorithm>

//----------------------------------------------------------------------------
// - Compute Manhattan Area
//----------------------------------------------------------------------------
// * distance : total (Manhattan) distance any given path in the area may be
// * source : initial position the paths within the area originate
// * predicate : a Boolean predicate describing whether a position may be
//      validly reached by another source position
//----------------------------------------------------------------------------
std::vector<sf::Vector2f> ManhattanArea::compute(int distance, const sf::Vector2f& source,
        std::function<bool(const sf::Vector2f&, const sf::Vector2f&)> predicate)
{
    std::vector<sf::Vector2f> area;
    adjacentReach(source, distance, source, predicate, area);        

    return area;
}
//----------------------------------------------------------------------------
// - AdjacentReach
//----------------------------------------------------------------------------
// * position : source of adjacency checks
// * distance : total (Manhattan) distance any given path in the area may be
// * previous : position that added this one (reduces back-tracking)
// * predicate : a Boolean predicate describing whether a position may be
//      validly reached by another source position
// * area : recursively built vector of all reachable positions in the area
//----------------------------------------------------------------------------
void ManhattanArea::adjacentReach(const sf::Vector2f& position, int distance, const sf::Vector2f& previous,
        std::function<bool(const sf::Vector2f&, const sf::Vector2f&)> predicate, std::vector<sf::Vector2f>& area)
{
    area.push_back(position);

    if(distance > 0)
    {
        // Check Rightward Position
        sf::Vector2f adjacent(position.x + 1, position.y);
        if(adjacent != previous && predicate(position, adjacent))
        {
            adjacentReach(adjacent, distance - 1, position, predicate, area);
        }

        // Check Leftward Position
        adjacent = sf::Vector2f(position.x - 1, position.y);
        if(adjacent != previous && predicate(position, adjacent))
        {
            adjacentReach(adjacent, distance - 1, position, predicate, area);
        }

        // Check Downward Position
        adjacent = sf::Vector2f(position.x, position.y + 1);
        if(adjacent != previous && predicate(position, adjacent))
        {
            adjacentReach(adjacent, distance - 1, position, predicate, area);
        }

        // Check Upward Position
        adjacent = sf::Vector2f(position.x, position.y - 1);
        if(adjacent != previous && predicate(position, adjacent))
        {
            adjacentReach(adjacent, distance - 1, position, predicate, area);
        }
    }
}