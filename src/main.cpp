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
#include "control/Cursor.h"
#include "game/InputManager.h"
#include "control/Menu.h"
#include "sprite/SpriteArea.h"
#include "sprite/SpriteActorHUD.h"

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
    sf::Texture assassin_texture, assassin_portrait;
    assassin_texture.loadFromFile("resources/graphics/Assassin.png");
    assassin_portrait.loadFromFile("resources/graphics/AssassinPortrait_64x104.png");
    
    Actor* assassin = new Actor(assassin_texture, &map);
    assassin->setPosition(sf::Vector3f(12, 12, map.height(12, 12)));
    map.addObject(assassin);

    assassin->setName("Assassin");
    assassin->setPortrait(assassin_portrait);
    
    SpriteActorHUD hud(*assassin);

    // Setup Paladin player
    sf::Texture paladin_texture;
    paladin_texture.loadFromFile("resources/graphics/Paladin.png");

    Actor* paladin = new Actor(paladin_texture, &map);
    map.addObject(paladin);

    paladin->setPosition(sf::Vector3f(5, 5, map.height(5, 5)));

    std::list<sf::Vector2f> patrol;
    for(int i = 0; i < 10; i++)
    {
        patrol.push_back(sf::Vector2f(10, 5));
        patrol.push_back(sf::Vector2f(10, 10));
        patrol.push_back(sf::Vector2f(5, 10));
        patrol.push_back(sf::Vector2f(5, 5));
    }

    paladin->walkAlong(patrol);

    // Cursor setup
    sf::Texture cursor_texture;
    cursor_texture.loadFromFile("resources/graphics/Cursor_32x16.png");
    sf::Sprite cursor_sprite(cursor_texture);
    cursor_sprite.setOrigin(cursor_texture.getSize().x / 2, cursor_texture.getSize().y / 2);
    

    Cursor* cursor = new Cursor(cursor_sprite, map, [](){std::cout << "Hey." << std::endl;});
    map.addObject(cursor);
    InputManager::instance().push(cursor);
    
    // Test Selector setup
    sf::Texture area_texture;
    area_texture.loadFromFile("resources/graphics/AreaSquare.png");

    std::vector<sf::Vector2f> area = assassin->reach();    
    
    SpriteArea* area_sprite = new SpriteArea(area_texture, area, map, sf::Color(140,140,255));

    map.addObject(area_sprite);

    sf::Clock timer;
    float timing;
    
    timer.restart();
    for(int i = 0; i < 1; i++)
    {
        assassin->reach();
    }
    timing = timer.restart().asMicroseconds();

    std::cout << "Execution time: " << timing << " us " << std::endl;
        
    // Background/Foreground panorama
    sf::Texture sky_texture;
    sky_texture.loadFromFile("resources/graphics/CloudySky.jpg");

    Panorama background(sky_texture);
    background.setPanningRate(0.2);

    sf::Color tints[4] = {sf::Color(0, 0, 0, 0), sf::Color(255, 0, 0, 60), sf::Color(0, 255, 0, 60), sf::Color(0, 0, 255, 60)};
    int tintIndex = 0;

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
  
        // Keyboard Input handles : T - Cycle Next Tint
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        {
            if(!view.tinting())
            {
                view.tint(tints[++tintIndex % 4], 1);
            }
        }

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
        InputManager::instance().poll();
        elapsed = clock.restart().asSeconds();
        Animations::instance().update(elapsed);
        window.clear(sf::Color::Black);
        background.drawOverlays(window);
        window.setView(view);
        window.draw(map);        
        window.draw(hud);
        view.drawOverlays(window);
        window.display();        
    }
    
    std::cout << "ok" << std::endl;

    return 0;
}