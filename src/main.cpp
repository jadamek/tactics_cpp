#include <iostream>
#include <SFML/Graphics.hpp>
#include "settings.h"
#include "map/Tile.h"
#include "map/Map.h"
#include "sprite/map/SpriteTile.h"
#include "objects/Actor.h"

int main()
{
    sf::Texture soul_texture, grass_texture, dirt_texture;

    grass_texture.loadFromFile("resources/graphics/GrassTile.png");
    dirt_texture.loadFromFile("resources/graphics/DirtTile.png");

    Map map(2, 2, MAP_SCALE);

    Tile* tiles[4];

    for(int x = 0; x < 2; x++)
    {
        for(int y = 0; y < 2; y++)
        {
            float height = 2;

            SpriteTile* tile_sprite = new SpriteTile(grass_texture, MAP_SCALE.x, MAP_SCALE.y, MAP_SCALE.z * height);
            Tile* tile = new Tile(tile_sprite, height);

            map.place(tile, x, y);
        }
    }

    soul_texture.loadFromFile("resources/graphics/Soul.png");    
    sf::Sprite soul_sprite(soul_texture);
    Actor soul(0, &map);

    // Window and view
    sf::RenderWindow window(sf::VideoMode(640, 480), "Tactics");

    sf::View view(window.getDefaultView());
    view.setCenter(0, 0);

    window.setView(view);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(map);
        window.display();
    }

    std::cout << "ok" << std::endl;

    return 0;
}