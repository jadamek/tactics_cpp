#include <iostream>
#include <SFML/Graphics.hpp>
#include "settings.h"
#include "map/Tile.h"
#include "sprite/map/SpriteTile.h"

int main()
{
    sf::Texture soul_texture, grass_texture;

    soul_texture.loadFromFile("resources/graphics/Soul.png");    
    sf::Sprite soul(soul_texture);

    grass_texture.loadFromFile("resources/graphics/GrassTile.png");
    SpriteTile grass_tile_sprite(grass_texture);
    grass_tile_sprite.resetHeight(32);

    Tile grass_tile(&grass_tile_sprite, 2.0);

    std::cout << "(" << grass_tile.getGlobalBounds().left
        << ", " << grass_tile.getGlobalBounds().top
        << ", " << grass_tile.getGlobalBounds().width
        << ", " << grass_tile.getGlobalBounds().height << ")" << std::endl;

    std::cout << grass_tile.getGlobalBounds().intersects(sf::FloatRect(200, 0, 40, 40)) << std::endl;

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
        window.draw(grass_tile);
        window.display();
    }

    std::cout << "ok" << std::endl;

    return 0;
}