#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "settings.h"
#include "map/Tile.h"
#include "map/Map.h"
#include "sprite/map/SpriteTile.h"
#include "objects/Actor.h"
#include "sprite/SpriteActor.h"

int main()
{
    sf::Texture soul_texture, grass_texture, dirt_texture;

    grass_texture.loadFromFile("resources/graphics/GrassTile.png");
    dirt_texture.loadFromFile("resources/graphics/DirtTile.png");

    Map map(2, 2, MAP_SCALE);

    for(int x = 0; x < 2; x++)
    {
        for(int y = 0; y < 2; y++)
        {
            float height = 1 + (rand() % 4);

            SpriteTile* tile_sprite = new SpriteTile(grass_texture, MAP_SCALE.x, MAP_SCALE.y, MAP_SCALE.z * height);
            Tile* tile = new Tile(tile_sprite, height);

            map.place(tile, x, y);
        }
    }

    soul_texture.loadFromFile("resources/graphics/Soul.png");    
    sf::Sprite* soul_basic_sprite = new sf::Sprite(soul_texture);
    SpriteActor* soul_sprite = new SpriteActor(soul_basic_sprite);
    soul_sprite->move(-12, -40);

    Actor soul(soul_sprite, &map);

    map.addObject(&soul);

    map.images_.update(1.f);

    // Window and view
    sf::RenderWindow window(sf::VideoMode(640, 480), "Tactics");

    sf::View view(window.getDefaultView());
    view.setCenter(0, 0);

    window.setView(view);

    // Clock and Animation
    sf::Clock clock;
    float elapsed = 0;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // Keyboard Input handles
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if(!soul.moving())
            {
                soul.moveTo(soul.position() + sf::Vector3f(-1.0, 0, 0));
            }
        }

        // Keyboard Input handles
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if(!soul.moving())
            {
                soul.moveTo(soul.position() + sf::Vector3f(1.0, 0, 0));
            }
        }

        // Keyboard Input handles
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if(!soul.moving())
            {
                soul.moveTo(soul.position() + sf::Vector3f(0, -1.0, 0));
            }
        }

        // Keyboard Input handles
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if(!soul.moving())
            {
                soul.moveTo(soul.position() + sf::Vector3f(0, 1.0, 0));
            }
        }

        // Timing updates
        elapsed = clock.restart().asSeconds();
        soul.update(elapsed);

        window.clear(sf::Color::Black);
        window.draw(map);
        window.display();
    }

    std::cout << "ok" << std::endl;

    return 0;
}