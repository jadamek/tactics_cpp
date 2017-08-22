#ifndef TACTICS_MANHATTAN_AREA_H
#define TACTICS_MANHATTAN_AREA_H

#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>

//================================================================================
// ** Manhattan Area
//================================================================================
// Utility class to recursively compute a an area of all paths within a given
// Manhattan distance obeying a given validation predicate
class ManhattanArea
{
// Methods
public:
    static std::vector<sf::Vector2f> compute(int distance, const sf::Vector2f& source,
                std::function<bool(const sf::Vector2f&, const sf::Vector2f&)> predicate);

private:
    static void adjacentReach(const sf::Vector2f& position, int distance, const sf::Vector2f& previous,
                std::function<bool(const sf::Vector2f&, const sf::Vector2f&)> predicate, std::vector<sf::Vector2f>& reachable);

    ManhattanArea(){}
    ManhattanArea(const ManhattanArea&){}
    ~ManhattanArea(){}
};

#endif