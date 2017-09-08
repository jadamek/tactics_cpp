#include "TargetConfirmer.h"
#include "../game/ActionScheduler.h"

//----------------------------------------------------------------------------
// - Target Confirmer Constructor
//----------------------------------------------------------------------------
// * cursor : sprite of the cursor to use for this selector
//----------------------------------------------------------------------------
TargetConfirmer::TargetConfirmer(const sf::Sprite& cursor) :
sprite_(cursor),
actionConfirm_([](){}),
actionMove_([](Actor*){}),
actionCancel_([](){}),
current_(0)
{
    setSpeed(20);
}

//----------------------------------------------------------------------------
// - Target Confirmer Constructor
//----------------------------------------------------------------------------
// * cursor : sprite of the cursor to use for this selector
// * targets : chosen victims of the skill to be cast
//----------------------------------------------------------------------------
TargetConfirmer::TargetConfirmer(const sf::Sprite& cursor, const std::vector<Actor*>& targets) :
TargetConfirmer(cursor)
{
    setTargets(targets);
}

//----------------------------------------------------------------------------
// - Target Confirmer Destructor
//----------------------------------------------------------------------------
TargetConfirmer::~TargetConfirmer()
{}

//----------------------------------------------------------------------------
// - Get Height (Override)
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the object
//----------------------------------------------------------------------------
float TargetConfirmer::getHeight(const sf::Vector2f& position) const
{
    return 0;
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this tile's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect TargetConfirmer::getGlobalBounds() const
{
    return sprite_.getGlobalBounds();
}

//----------------------------------------------------------------------------
// Poll Input Event (Override)
//----------------------------------------------------------------------------
void TargetConfirmer::poll()
{    
    // Cannot cycle through an empty target list
    if(!targets_.empty()){
        // Keyboard Input handle : Down - move cursor to previous target
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            Actor* next = targets_[(current_ - 1) % targets_.size()];
            moveTo(next->position());
            actionMove_(next);
        }

        // Keyboard Input handle : Up - move cursor to next target
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {           
            Actor* next = targets_[(current_ + 1) % targets_.size()];
            moveTo(next->position());
            actionMove_(next);
        }

        // Keyboard Input handle : Left - move cursor to previous target
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            Actor* next = targets_[(current_ - 1) % targets_.size()];
            moveTo(next->position());
            actionMove_(next);
        }

        // Keyboard Input handle : Right - move cursor  to next target
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            Actor* next = targets_[(current_ + 1) % targets_.size()];
            moveTo(next->position());
            actionMove_(next);
        }
    }

    // Keyboard Input handle : Enter|Space - confirm cast
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        actionConfirm_();
    }

    // Keyboard Input handle : Esc - cancel menu
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        actionCancel_();
    }
}

//----------------------------------------------------------------------------
// Cursor Busy? (Override)
//----------------------------------------------------------------------------
bool TargetConfirmer::busy() const
{
    return busy_ || moving();
}

//----------------------------------------------------------------------------
// Set Confirm Action
//----------------------------------------------------------------------------
// * action : callable to execute when the target is confirmed
//----------------------------------------------------------------------------
void TargetConfirmer::setOnConfirm(std::function<void()> action)
{
    actionConfirm_ = action;
}

//----------------------------------------------------------------------------
// Set Cancel Action
//----------------------------------------------------------------------------
// * action : callable to execute when the 'go back' (esc, e.g) is pressed
//----------------------------------------------------------------------------
void TargetConfirmer::setOnCancel(std::function<void()> action)
{
    actionCancel_ = action;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void TargetConfirmer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(active()){
        target.draw(sprite_, states);        
    }
}

//----------------------------------------------------------------------------
// Set Move Action
//----------------------------------------------------------------------------
// * action : callable to execute when the selector moves to the next target
//----------------------------------------------------------------------------
void TargetConfirmer::setOnMove(std::function<void(Actor*)> action)
{
    actionMove_ = action;
}

//----------------------------------------------------------------------------
// Get Affected Targets
//----------------------------------------------------------------------------
const std::vector<Actor*>& TargetConfirmer::getTargets() const
{
    return targets_;
}

//----------------------------------------------------------------------------
// Compute Affected Targets
//----------------------------------------------------------------------------
// * skill : skill being confirmed for casting
// * target : chosen position of the skill's cast source
//----------------------------------------------------------------------------
void TargetConfirmer::setTargets(const std::vector<Actor*>& targets)
{
    targets_ = targets;

    if(!targets_.empty())
    {
        // Start cursor on the first victim
        setPosition(targets_[0]->position());    
    }
}