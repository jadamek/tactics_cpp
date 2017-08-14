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
#include "screen/ViewEx.h"
#include "screen/Panorama.h"
#include "sprite/SpriteDirected.h"
#include "sprite/SpriteAnimated.h"
#include "game/ActionScheduler.h"

int main()
{
    sf::Texture soul_texture, grass_texture, dirt_texture;

    grass_texture.loadFromFile("resources/graphics/GrassTile_32x16.png");
    dirt_texture.loadFromFile("resources/graphics/DirtTile_32x16.png");

    Map map(15, 15);

    for(int x = 0; x < map.width(); x++)
    {
        for(int y = 0; y < map.length(); y++)
        {
            float height = 3;

            SpriteTile* tile_sprite = new SpriteTile(grass_texture, MAP_SCALE.x, MAP_SCALE.y, MAP_SCALE.z * height);
            Tile* tile = new Tile(tile_sprite, height);

            map.place(tile, x, y);
        }
    }

    // Setup Assassin player
    sf::Texture assassin_texture;
    assassin_texture.loadFromFile("resources/graphics/Assassin.png");

    Actor* assassin = new Actor(assassin_texture, &map);
    map.addObject(assassin);

    // Setup Paladin player
    sf::Texture paladin_texture;
    paladin_texture.loadFromFile("resources/graphics/Paladin.png");

    Actor* paladin = new Actor(paladin_texture, &map);
    map.addObject(paladin);

    paladin->setPosition(sf::Vector3f(5, 5, map.height(5, 5)));

    // Background/Foreground panorama
    sf::Texture sky_texture;
    sky_texture.loadFromFile("resources/graphics/CloudySky.jpg");

    Panorama background(sky_texture);
    background.setPanningRate(0.2);
    
    // Window and view
    sf::RenderWindow window(sf::VideoMode(640, 480), "Tactics");

    ViewEx view;
    view.setCenter(0, 0);

    window.setView(view);

    view.fadeIn(3);

    // Clock and Animation
    sf::Clock clock;
    float elapsed = 0, sort_time;

    clock.restart();
    map.getDepthBuffer().sort();

    bool closing = false;

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        
        // Keyboard Input handles : Left - Move | SHIFT + Left - Scroll Left
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                if(!view.scrolling() && !view.focusing())
                {
                    view.scroll(sf::Vector2f(-1 * MAP_SCALE.x, 0), 0.3);
                }
            }
            else if(!assassin->walking())
            {
                assassin->walk(sf::Vector2f(-1.0, 0));
            }
        }

        // Keyboard Input handles : Right - Move | SHIFT + Right - Scroll Right
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                if(!view.scrolling() && !view.focusing())
                {
                    view.scroll(sf::Vector2f(MAP_SCALE.x, 0), 0.3);
                }
            }
            else if(!assassin->walking())
            {
                assassin->walk(sf::Vector2f(1.0, 0));
            }
        }

        // Keyboard Input handles : Up - Move | SHIFT + Up - Scroll Up
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                if(!view.scrolling() && !view.focusing())
                {
                    view.scroll(sf::Vector2f(0, -1 * MAP_SCALE.y), 0.3);
                }
            }
            else if(!assassin->walking())
            {
                assassin->walk(sf::Vector2f(0, -1.0));
            }
        }

        // Keyboard Input handles : Down - Move | SHIFT + Down - Scroll down
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                if(!view.scrolling() && !view.focusing())
                {
                    view.scroll(sf::Vector2f(0, MAP_SCALE.y), 0.3);
                }
            }
            else if(!assassin->walking())
            {
                assassin->walk(sf::Vector2f(0, 1.0));
            }
        }

        // Keyboard Input handles : Q - Shake | CTRL + Q - Stop Shake
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            {
                view.stopShaking();
            }
            else if(!view.shaking())
            {
                view.shake(16.0, 2);
            }
        }

        // Keyboard Input handles : S - Spin | CTRL + S - Stop Spin
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            {
                view.stopSpinning();
            }
            else if(!view.spinning())
            {
                view.spin(0.5);
            }
        }

        // Keyboard Input handles : (Numpad +) - Zoom in
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add) && !view.zooming())
        {
            view.zoom(-0.25, 2);
        }

        // Keyboard Input handles : (Numpad -) - Zoom out
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract) && !view.zooming())
        {
            view.zoom(0.25, 2);
        }

        /*        
        // Keyboard Input handles : T - Cycle Next Tint
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {

        }
        */

        // Keyboard Input handles : F - Flash
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F) && !view.flashing())
        {
            view.flash(1, sf::Color(80, 80, 160));
        }

        // Keyboard Input handles : G - Goto Player (Focus)
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::G) && !view.focusing())
        {
            view.focus(assassin, 0.5);
        }
        
        // Keyboard Input handles : Escape
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            view.fadeOut(1);
            closing = true;
        }

        if(closing && !view.tinting())
        {
            window.close();
        }

        // Timing updates
        elapsed = clock.restart().asSeconds();
        ActionScheduler::instance().update(elapsed);
        assassin->update(elapsed);
        paladin->update(elapsed);
        map.getDepthBuffer().update(elapsed);
        background.update(elapsed);
        view.update(elapsed);

        window.clear(sf::Color::Black);
        background.drawOverlays(window);
        window.setView(view);
        window.draw(map);
        view.drawOverlays(window);
        window.display();
    }

    std::cout << "ok" << std::endl;

    return 0;
}