#include "Cursor.h"

//----------------------------------------------------------------------------
// - Cursor Constructor
//----------------------------------------------------------------------------
// * cursor : sprite used as the visual cursor
// * map : map this cursor belongs to and moves within
// * action, callable to execute when the cursor is "clicked"
//----------------------------------------------------------------------------
Cursor::Cursor(const sf::Sprite& cursor, Map& map) :
    map_(&map),
    sprite_(cursor),
    throttle_(0)
{  
    setPosition(sf::Vector3f(0, 0, std::max(0.f, map_->height(0, 0))));
    setSpeed(20);
}

//----------------------------------------------------------------------------
// - Cursor Destructor
//----------------------------------------------------------------------------
Cursor::~Cursor()
{}

//----------------------------------------------------------------------------
// - Go To Position
//----------------------------------------------------------------------------
// * position : (x,y) position on the map to move to
//----------------------------------------------------------------------------
void Cursor::goTo(const sf::Vector2f& position)
{
    if(map_->valid(position.x, position.y))
    {
        sf::Vector3f target(position.x, position.y, map_->height(position.x, position.y));
        moveTo(target);
        actionMove_(target);        
    }
}

//----------------------------------------------------------------------------
// - Get Height (Override)
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the object
//----------------------------------------------------------------------------
float Cursor::getHeight(const sf::Vector2f& position) const
{
    return 0;
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this tile's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect Cursor::getGlobalBounds() const
{
    return sprite_.getGlobalBounds();
}

//----------------------------------------------------------------------------
// Poll Input Event (Override)
//----------------------------------------------------------------------------
void Cursor::poll()
{
    // Consecutive keyboard input for cursors is throttled by 5 frames
    static int throttle = 10;
    
    
    if(throttle_ <= 0)
    {
        // Keyboard Input handle : Down - move cursor downward
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {           
            // Move to the next valid position downward of this
            int y = position().y + 1;
            while(!map_->valid(position().x, y) && y < map_->length())
            {
                y++;
            }

            if(y < map_->length())
            {
                goTo(sf::Vector2f(position().x, y));
                throttle_ = throttle;                
            }
        }

        // Keyboard Input handle : Up - move cursor upward
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {           
            // Move to the next valid position upward of this
            int y = position().y - 1;
            while(!map_->valid(position().x, y) && y >= 0)
            {
                y--;
            }

            if(y >= 0)
            {
                goTo(sf::Vector2f(position().x, y));
                throttle_ = throttle;                
            }
        }

        // Keyboard Input handle : Left - move cursor left
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            // Move to the next valid position left of this
            int x = position().x - 1;
            while(!map_->valid(x, position().y) && x >= 0)
            {
                x--;
            }

            if(x >= 0)
            {
                goTo(sf::Vector2f(x, position().y));
                throttle_ = throttle;                
            }
        }

        // Keyboard Input handle : Right - move cursor right
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // Move to the next valid position right of this
            int x = position().x + 1;
            while(!map_->valid(x, position().y) && x < map_->width())
            {
                x++;
            }

            if(x < map_->width())
            {
                goTo(sf::Vector2f(x, position().y));
                throttle_ = throttle;                
            }
        }

        // Keyboard Input handle : Enter - select current position
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            actionConfirm_(position());
            throttle_ = throttle;            
        }

        // Keyboard Input handle : Esc - cancel selector
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            actionCancel_();
            throttle_ = throttle;            
        }
    }    
}

//----------------------------------------------------------------------------
// Cursor Busy? (Override)
//----------------------------------------------------------------------------
bool Cursor::busy() const
{
    return busy_ || moving();
}

//----------------------------------------------------------------------------
// Set Confirm Action
//----------------------------------------------------------------------------
// * action : callable to execute when the cursor selects a position
//----------------------------------------------------------------------------
void Cursor::setOnConfirm(std::function<void(const sf::Vector3f&)> action)
{
    actionConfirm_ = action;
}

//----------------------------------------------------------------------------
// Set Cancel Action
//----------------------------------------------------------------------------
// * action : callable to execute when the 'go back' (esc, e.g) is pressed
//----------------------------------------------------------------------------
void Cursor::setOnCancel(std::function<void()> action)
{
    actionCancel_ = action;
}

//----------------------------------------------------------------------------
// Set Move Action
//----------------------------------------------------------------------------
// * action : callable to execute when the cursor moves to a target position
//----------------------------------------------------------------------------
void Cursor::setOnMove(std::function<void(const sf::Vector3f&)> action)
{
    actionMove_ = action;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void Cursor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(active()){
        target.draw(sprite_, states);        
    }
}

//----------------------------------------------------------------------------
// - Increment Frame (Override)
//----------------------------------------------------------------------------
void Cursor::step()
{
    MobileObject::step();

    if(throttle_ > 0)
    {
        throttle_--;
    }
}