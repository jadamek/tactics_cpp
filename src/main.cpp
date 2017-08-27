#include "game/Scene.h"
#include <iostream>

int main()
{
    std::cout << "Starting game ... " << std::endl;

    // Main game window
    sf::RenderWindow window(sf::VideoMode(640, 480), "Tactics!");
    
    // Main game clock
    sf::Clock clock;
    float elapsed = 0;

    // Initiate Scene
    Scene scene;
    scene.start();
    
    while(window.isOpen())
    {
        if(scene.closed())
        {
            window.close();
        }
        
        // Basic window event handling
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                scene.close();
        }

        // Timing updates
        elapsed = clock.restart().asSeconds();

        // Draw calls
        window.clear(sf::Color::Black);
        window.draw(scene);
        window.display();
    }
    
    std::cout << "Exiting game ... " << std::endl;
    
    return 0;
}