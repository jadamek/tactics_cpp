#include "control/Menu.h"
#include "control/Cursor.h"
#include "map/Map.h"
#include <iostream>

int main()
{
    sf::Clock timer;
    float elapsed;
    sf::Texture texture;
    texture.create(40, 20);
    sf::Sprite csprite(texture);
    Map map;
    Cursor* cursor;

    timer.restart();
    for(int i = 0; i < 10000; i++)
    {
        cursor = new Cursor(csprite, map);
    }
    elapsed = timer.restart().asMicroseconds();

    std::cout << elapsed / 10000.f << " us " << std::endl;
    return 0;
}