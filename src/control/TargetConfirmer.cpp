#include "TargetConfirmer.h"
#include "../game/ActionScheduler.h"

//----------------------------------------------------------------------------
// - Target Confirmer Constructor
//----------------------------------------------------------------------------
// * texture : bitmap of the cursor to use for this selector
// * skill : skill being confirmed for casting
// * caster : actor casting the skill in question
// * targets : chosen targets to confirm
//----------------------------------------------------------------------------
TargetConfirmer::TargetConfirmer(const sf::Texture& texture, Skill* skill, Actor* caster, const std::vector<Actor*>& targets) :
sprite_(texture),
throttle_(0),
skill_(skill),
caster_(caster),
targets_(targets),
current_(0)
{    
    sprite_.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

    if(!targets_.empty())
    {
        setPosition(targets_[0]->position());    
    }
    else if(caster_){
        setPosition(caster_->position());
    }

    setSpeed(10);
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
    // Consecutive keyboard input for cursors is throttled by 5 frames
    static int throttle = 10;   
    
    if(skill_ != 0 && caster_ != 0){
        if(throttle_ <= 0)
        {
            // Cannot cycle through an empty target list
            if(!targets_.empty()){
                // Keyboard Input handle : Down - move cursor to previous target
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {           
                    moveTo(targets_[(current_ - 1) % targets_.size()]->position());
                    throttle_ = throttle;                
                }

                // Keyboard Input handle : Up - move cursor to next target
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {           
                    moveTo(targets_[(current_ + 1) % targets_.size()]->position());
                    throttle_ = throttle;                
                }

                // Keyboard Input handle : Left - move cursor to previous target
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    moveTo(targets_[(current_ - 1) % targets_.size()]->position());
                    throttle_ = throttle;                
                }

                // Keyboard Input handle : Right - move cursor  to next target
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    moveTo(targets_[(current_ + 1) % targets_.size()]->position());
                    throttle_ = throttle;                
                }
            }

            // Keyboard Input handle : Enter - confirm cast
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                skill_->use(caster_, targets_);
                //setBusy(true);
            }
        }
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
// - Draw (Override)
//----------------------------------------------------------------------------
void TargetConfirmer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(active()){
        target.draw(sprite_, states);        
    }
}

//----------------------------------------------------------------------------
// - Increment Frame (Override)
//----------------------------------------------------------------------------
void TargetConfirmer::step()
{
    MobileObject::step();

    if(throttle_ > 0)
    {
        throttle_--;
    }
}