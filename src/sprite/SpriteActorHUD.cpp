#include "SpriteActorHUD.h"

//----------------------------------------------------------------------------
// - Sprite Actor HUD Default Constructor
//----------------------------------------------------------------------------
SpriteActorHUD::SpriteActorHUD() :
portrait_(0),
body_(sf::Vector2f(180, 80)),
visible_(true)
{
    font_.loadFromFile("resources/fonts/Arial.ttf");

    body_.setPosition(0, 40);
    body_.setFillColor(sf::Color(50, 50, 170, 200));
}

//----------------------------------------------------------------------------
// - Sprite Actor HUD Actor Constructor
//----------------------------------------------------------------------------
// * actor : player this HUD displays information for
//----------------------------------------------------------------------------
SpriteActorHUD::SpriteActorHUD(const Actor& actor) :
    SpriteActorHUD()
{    
    setActor(actor);
}

//----------------------------------------------------------------------------
// - Sprite Actor HUD Destructor
//----------------------------------------------------------------------------
SpriteActorHUD::~SpriteActorHUD()
{}

//----------------------------------------------------------------------------
// - Set Actor
//----------------------------------------------------------------------------
// * actor : player this HUD displays information for
//----------------------------------------------------------------------------
void SpriteActorHUD::setActor(const Actor& actor)
{
    // Portrait
    portrait_ = actor.getPortrait();

    if(portrait_)
    {
        portrait_->setPosition(0, 16);        
    }

    // Level
    lvl_ = sf::Text("Lvl 1", font_, 12);
    lvl_.setStyle(sf::Text::Bold);
    lvl_.setPosition(4, 112 - lvl_.getGlobalBounds().height);

    // Name
    name_ = sf::Text(actor.getName(), font_, 12);
    name_.setStyle(sf::Text::Bold);
    name_.setPosition(124 - name_.getGlobalBounds().width / 2.f, 52);

    // HP
    hpLabel_ = sf::Text("HP", font_, 12);
    hpLabel_.setStyle(sf::Text::Bold);
    hpLabel_.setPosition(72, 76);
    hpValue_ = sf::Text("100 / 100", font_, 12);
    hpValue_.setStyle(sf::Text::Bold);
    hpValue_.setPosition(168 - hpValue_.getGlobalBounds().width, 76);

    // MP
    mpLabel_ = sf::Text("MP", font_, 12);
    mpLabel_.setStyle(sf::Text::Bold);
    mpLabel_.setPosition(72, 96);
    mpValue_ = sf::Text("10 / 10", font_, 12);
    mpValue_.setStyle(sf::Text::Bold);
    mpValue_.setPosition(168 - mpValue_.getGlobalBounds().width, 96);
}

//----------------------------------------------------------------------------
// - HUD Is Visible?
//----------------------------------------------------------------------------
bool SpriteActorHUD::visible() const
{
    return visible_;
}

//----------------------------------------------------------------------------
// - Show HUD
//----------------------------------------------------------------------------
void SpriteActorHUD::show()
{
    visible_ = true;
}

//----------------------------------------------------------------------------
// - Hide HUD
//----------------------------------------------------------------------------
void SpriteActorHUD::hide()
{
    visible_ = false;
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle
//----------------------------------------------------------------------------
// Returns the rectangle this HUD inscribes
//----------------------------------------------------------------------------
sf::FloatRect SpriteActorHUD::getGlobalBounds() const
{
    return sf::FloatRect(0, 0, 180, 120);
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void SpriteActorHUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(visible_)
    {
        states.transform *= getTransform();
        target.draw(body_, states);

        if(portrait_)
        {
            target.draw(*portrait_, states);
        }
    
        target.draw(name_, states);
        target.draw(lvl_, states);
        target.draw(hpLabel_, states);
        target.draw(hpValue_, states);
        target.draw(mpLabel_, states);
        target.draw(mpValue_, states);
    }
}
