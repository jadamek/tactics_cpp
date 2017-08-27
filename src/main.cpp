#include "game/Scene.h"
#include "game/InputManager.h"
#include "game/Animations.h"
#include <iostream>

//================================================================================
// ** Tactics Main Game Loop
//================================================================================
// The main program loop. Here, the Scene is created, setup and drawn. Animations
// are brought to life using the central game clock, and input is redirected to
// the input manager using polling
//================================================================================
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
        InputManager::instance().poll();
        elapsed = clock.restart().asSeconds();
        Animations::instance().update(elapsed);

        // Draw calls
        window.clear(sf::Color::Black);
        window.draw(scene);
        window.display();
    }
    
    std::cout << "Exiting game ... " << std::endl;
    
    return 0;
}