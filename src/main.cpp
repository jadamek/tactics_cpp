#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>
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

    Map map(35, 35, MAP_SCALE);

    for(int x = 0; x < map.width(); x++)
    {
        for(int y = 0; y < map.length(); y++)
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

    Actor* soul = new Actor(soul_sprite, &map);
    map.addObject(soul);
    
    // Window and view
    sf::RenderWindow window(sf::VideoMode(640, 480), "Tactics");

    sf::View view(window.getDefaultView());
    view.setCenter(0, 0);

    window.setView(view);

    // Clock and Animation
    sf::Clock clock;
    float elapsed = 0, sort_time;

    clock.restart();
    map.getDepthBuffer().sort();
    sort_time = clock.restart().asMicroseconds();

    std::cout << "Full sort in : " << sort_time << " us " << std::endl;

    bool closing = false;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        
        // Keyboard Input handles : LEFT
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if(!soul->moving())
            {
                soul->moveTo(soul->position() + sf::Vector3f(-1.0, 0, 0));
            }
        }

        // Keyboard Input handles : RIGHT
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if(!soul->moving())
            {
                soul->moveTo(soul->position() + sf::Vector3f(1.0, 0, 0));
            }
        }

        // Keyboard Input handles : UP
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if(!soul->moving())
            {
                soul->moveTo(soul->position() + sf::Vector3f(0, -1.0, 0));
            }
        }

        // Keyboard Input handles : DOWN
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if(!soul->moving())
            {
                soul->moveTo(soul->position() + sf::Vector3f(0, 1.0, 0));
            }
        }
        
        // Keyboard Input handles : ESC
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            closing = true;
        }

        if(closing)
        {
            window.close();
        }

        // Timing updates
        elapsed = clock.restart().asSeconds();
        soul->update(elapsed);
        map.getDepthBuffer().update(elapsed);

        window.clear(sf::Color::Black);
        window.draw(map);
        window.display();
    }

    std::cout << "ok" << std::endl;

    return 0;
}