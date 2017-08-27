#include "Menu.h"
#include "../settings.h"
#include <math.h>

//----------------------------------------------------------------------------
// - Menu Constructor
//----------------------------------------------------------------------------
// * frameTexture : bitmap to use for the menu's frame (replace w/ Menu::Style)
//----------------------------------------------------------------------------
Menu::Menu(const sf::Texture& frameTexture) :
    AnimatedObject(FPS),
    current_(0),
    throttle_(0),
    frame_(frameTexture),
    width_(32),
    body_(sf::Vector2f(32, 32))
{
    body_.setFillColor(sf::Color::Blue);
    font_.loadFromFile("resources/fonts/Arial.ttf");
}

//----------------------------------------------------------------------------
// - Menu Destructor
//----------------------------------------------------------------------------
Menu::~Menu()
{}

//----------------------------------------------------------------------------
// - Add Menu Option
//----------------------------------------------------------------------------
// * label : text displayed as the option's label
// * action : callback executed when the option is selected
//----------------------------------------------------------------------------
void Menu::addOption(const std::string& label, std::function<void()> action)
{
    sf::Text labelSprite(label, font_, 16);
    labelSprite.setPosition(4, 20 * options_.size() + 4);
    labelSprite.setStyle(sf::Text::Bold);\

    // Increase the width of the menu to hold a bolded label
    unsigned int textWidth = ceil(labelSprite.getGlobalBounds().width);
    if(textWidth > width_)
    {
        width_ = textWidth;
    }
    
    if(!options_.empty())
    {
        labelSprite.setStyle(sf::Text::Regular);
    }

    options_.push_back(std::pair<sf::Text, std::function<void()>>(labelSprite, action));

    // Update sprites to accommodate new option
    body_.setSize(sf::Vector2f(width_ + 8, options_.size() * 20 + 12));
    frame_.setSize(sf::Vector2u(width_ + 8, options_.size() * 20 + 12));
}

#include <iostream>

//----------------------------------------------------------------------------
// - Handle Input (Override)
//----------------------------------------------------------------------------
void Menu::poll()
{
    // Consecutive keyboard input for menus is throttled by 10 frames
    static int throttle = 15;

    // Consecutive keyboard input is throttled by 20 frames
    if(throttle_ <= 0)
    {
        // Keyboard Input handle : Down - highlight next option
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            highlight((current_ + 1) % options_.size());            
            throttle_ = throttle;
        }

        // Keyboard Input handle : Up - highlight previous option
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            int previous = current_ - 1;
            if(previous < 0)
            {
                previous = options_.size() - 1;
            }

            highlight(previous);

            throttle_ = throttle;            
        }

        // Keyboard Input handle : Enter - select current option
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            if(!options_.empty())
            {
                options_[current_].second();
            }
            throttle_ = throttle;            
        }

        // Keyboard Input handle : Esc - cancel menu
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            actionCancel_();
            throttle_ = throttle;            
        }
    }
}

//----------------------------------------------------------------------------
// Menu Busy? (Override)
//----------------------------------------------------------------------------
bool Menu::busy() const
{
    return busy_;
}

//----------------------------------------------------------------------------
// Set Cancel Action
//----------------------------------------------------------------------------
// * action : callable to execute when the 'go back' (esc, e.g) is pressed
//----------------------------------------------------------------------------
void Menu::setOnCancel(std::function<void()> action)
{
    actionCancel_ = action;
}

//----------------------------------------------------------------------------
// - Increment Frame (Override)
//----------------------------------------------------------------------------
void Menu::step()
{
    if(throttle_ > 0)
    {
        throttle_--;
    }
}

//----------------------------------------------------------------------------
// - Highlight Option
//----------------------------------------------------------------------------
// * optionIndex : index in the options vector to highlight as selected
//----------------------------------------------------------------------------
void Menu::highlight(int optionIndex)
{
    if((optionIndex < options_.size() && optionIndex >= 0) && optionIndex != current_)
    {
        options_[current_].first.setStyle(sf::Text::Regular);

        current_ = optionIndex;
        options_[current_].first.setStyle(sf::Text::Bold);
    }
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(active() && !options_.empty())
    {
        states.transform *= getTransform();
        target.draw(frame_, states);
        target.draw(body_, states);
        
        for(int i = 0; i < options_.size(); i++)
        {
            target.draw(options_[i].first);
        }
    }
}
