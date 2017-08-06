#include "SpriteAnimated.h"

//----------------------------------------------------------------------------
// - Animated Sprite Constructor
//----------------------------------------------------------------------------
// * sprite : indexed sprite on which all animations are played
//----------------------------------------------------------------------------
SpriteAnimated::SpriteAnimated(SpriteIndexed* sprite) :
    AnimatedObject(1),
    current_(0),
    playing_(""),
    sprite_(sprite)
{    
    if(sprite_){
        // Add default sequence -> 0 .. index limit
        std::vector<int> defaultSequence(sprite_->getIndexLimit());
        for(int i = 0; i < defaultSequence.size(); i++)
        {
            defaultSequence[i] = i;
        }

        add("default", defaultSequence);
    }
}

//----------------------------------------------------------------------------
// - Animated Sprite Destructor
//----------------------------------------------------------------------------
SpriteAnimated::~SpriteAnimated()
{
    if(sprite_)
    {
        delete sprite_;
    }
}

//----------------------------------------------------------------------------
// - Add Animation Sequence
//----------------------------------------------------------------------------
// * name : name of the sequence used to identify it within the sprite
// * sequence : vector of indices that comprises the animation
// Adds a new animation sequence to this sprite, which will be iterated
// through each frame while playing
// NOTE: will override an sequence of the same name if it exists
//----------------------------------------------------------------------------
void SpriteAnimated::add(const std::string& name, const std::vector<int>& sequence)
{
    if(!sequence.empty() && name != "")
    {
        animations_[name] = sequence;
    }
}

//----------------------------------------------------------------------------
// - Has Animation?
//----------------------------------------------------------------------------
// * name : name of the animation to search for
// Returns whether the sprite has a sequence assigned to the name given
//----------------------------------------------------------------------------
bool SpriteAnimated::has(const std::string& name) const
{
    return animations_.find(name) != animations_.end();
}

//----------------------------------------------------------------------------
// - Remove Animation
//----------------------------------------------------------------------------
// * name : name of the sequence to remove from the sprite
//----------------------------------------------------------------------------
void SpriteAnimated::remove(const std::string& name)
{
    if(playing_ == name)
    {
        stop();
    }

    animations_.erase(name);    
}

//----------------------------------------------------------------------------
// - Play Animation
//----------------------------------------------------------------------------
// * name : name of the sequence to begin playing
// * looping : animation will continue playing from the beginning after
//      reaching the final frame if this is set
//----------------------------------------------------------------------------
void SpriteAnimated::play(const std::string& name, bool looping)
{
    current_ = 0;
    playing_ = name;
    loop_ = looping;
}

//----------------------------------------------------------------------------
// - Animation Currently Playing?
//----------------------------------------------------------------------------
bool SpriteAnimated::playing() const
{
    return playing_ != "";
}

//----------------------------------------------------------------------------
// - Stop Playing Animation
//----------------------------------------------------------------------------
void SpriteAnimated::stop()
{
    playing_ = "";
}

//----------------------------------------------------------------------------
// - Get Sprite (Mutable)
//----------------------------------------------------------------------------
SpriteIndexed* SpriteAnimated::getSprite()
{
    return sprite_;
}

//----------------------------------------------------------------------------
// - Get Sprite (Constant)
//----------------------------------------------------------------------------
const SpriteIndexed* SpriteAnimated::getSprite() const
{
    return sprite_;
}

//----------------------------------------------------------------------------
// - Set Sprite
//----------------------------------------------------------------------------
// * sprite : indexed sprite which animations will be played on
//----------------------------------------------------------------------------
void SpriteAnimated::setSprite(SpriteIndexed* sprite)
{
    sprite_ = sprite;
    stop();
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect SpriteAnimated::getGlobalBounds() const
{
    if(sprite_)
    {
        return getTransform().transformRect(sprite_->getGlobalBounds());
    }
    
    return sf::FloatRect();
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void SpriteAnimated::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(sprite_)
    {
        states.transform *= getTransform();
        target.draw(*sprite_, states);
    }
}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
void SpriteAnimated::step()
{
    if(playing_ != "")
    {
        if(current_ < animations_[playing_].size())
        {
            sprite_->setIndex(animations_[playing_][current_++]);
        }

        if(current_ == animations_[playing_].size())
        {
            if(loop_)
            {
                current_ = 0;
            }
            else
            {
                stop();
            }
        }
    }
}