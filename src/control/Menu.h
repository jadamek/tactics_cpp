#ifndef TACTICS_MENU_H
#define TACTICS_MENU_H

#include "InputHandler.h"
#include "../objects/AnimatedObject.h"
#include "../sprite/SpriteMenuFrame.h"
#include "../sprite/Sprite.h"
#include <functional>
#include <string>
#include <vector>

//================================================================================
// ** Menu
//================================================================================
// Simple menu with a set of actions that may selected using the keyboard or mouse
//================================================================================
class Menu : public InputHandler, public Sprite, public AnimatedObject
{
// Methods
public:
    Menu(const sf::Texture& frameTexture);
    virtual ~Menu();

    void                    addOption(const std::string& label, std::function<void()> action);
    void                    clear();
    virtual void            poll();
    virtual bool            busy() const;
    void                    setOnCancel(std::function<void()> action);    
    virtual sf::FloatRect   getGlobalBounds() const;
protected:
    void                    step();
    void                    highlight(int optionIndex);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
// Members
    int                     current_;
    int                     throttle_;
    SpriteMenuFrame         frame_;
    sf::RectangleShape      body_;
    unsigned int            width_;
    sf::Font                font_;
    std::function<void()>   actionCancel_;    
    std::vector<std::pair<sf::Text, std::function<void()>>> options_;
};

#endif