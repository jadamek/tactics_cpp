#include "ManhattanArea.h"
#include <algorithm>

//----------------------------------------------------------------------------
// - Compute Manhattan Area
//----------------------------------------------------------------------------
// * distance : total (Manhattan) distance any given path in the area may be
// * source : initial position the paths within the area originate
// * passable : a Boolean predicate describing whether a position may be
//      validly reached by another source position
// * exclude : a Boolean predicate describing whether a position should be
//      excluded from the range, but still traversed over
//----------------------------------------------------------------------------
std::vector<sf::Vector2f> ManhattanArea::compute(int distance, const sf::Vector2f& source,
        std::function<bool(const sf::Vector2f&, const sf::Vector2f&)> passable,
        std::function<bool(const sf::Vector2f&)> exclude)
{
    std::vector<bool> visited((2 * distance + 1) * (2 * distance + 1), false);
    std::vector<sf::Vector2f> area;
    adjacentReach(source, distance, passable, exclude, area, source - sf::Vector2f(distance, distance), 2 * distance + 1, visited);

    return area;
}

//----------------------------------------------------------------------------
// - AdjacentReach
//----------------------------------------------------------------------------
// * position : source of adjacency checks
// * distance : total (Manhattan) distance any given path in the area may be
// * passable : a Boolean predicate describing whether a position may be
//      validly reached by another source position
// * exclude : a Boolean predicate describing whether a position should be
//      excluded from the range, but still traversed over
// * area : recursively built vector of all reachable positions in the area
// * origin : position which maps to the zero index of the visited array
// * width : width of the total area being covered for traversal
// * visited : array of visited nodes within the traversal area
//----------------------------------------------------------------------------
void ManhattanArea::adjacentReach(const sf::Vector2f& position, int distance,
        std::function<bool(const sf::Vector2f&, const sf::Vector2f&)> passable,
        std::function<bool(const sf::Vector2f&)> exclude, std::vector<sf::Vector2f>& area,
        const sf::Vector2f& origin, int width, std::vector<bool>& visited)
{    
    if(!exclude(position) && !visited[int(position.x - origin.x) + int(position.y - origin.y) * width])
    {
        area.push_back(position);
        visited[int(position.x - origin.x) + int(position.y - origin.y) * width] = true;
    }

    if(distance > 0)
    {
        // Check Rightward Position
        sf::Vector2f adjacent(position.x + 1, position.y);
        if((adjacent.x >= 0 && adjacent.y >= 0) && passable(position, adjacent))
        {
            adjacentReach(adjacent, distance - 1, passable, exclude, area, origin, width, visited);
        }
        
        // Check Leftward Position
        adjacent = sf::Vector2f(position.x - 1, position.y);
        if((adjacent.x >= 0 && adjacent.y >= 0) && passable(position, adjacent))
        {
            adjacentReach(adjacent, distance - 1, passable, exclude, area, origin, width, visited);
        }

        // Check Downward Position
        adjacent = sf::Vector2f(position.x, position.y + 1);
        if((adjacent.x >= 0 && adjacent.y >= 0) && passable(position, adjacent))
        {
            adjacentReach(adjacent, distance - 1, passable, exclude, area, origin, width, visited);
        }

        // Check Upward Position
        adjacent = sf::Vector2f(position.x, position.y - 1);
        if((adjacent.x >= 0 && adjacent.y >= 0) && passable(position, adjacent))
        {
            adjacentReach(adjacent, distance - 1, passable, exclude, area, origin, width, visited);
        }
    }
    
}