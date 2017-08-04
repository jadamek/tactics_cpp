#include "Panorama.h"
#include "../settings.h"
#include <algorithm>
#include <math.h>

# define M_PI 3.14159265358979323846

//----------------------------------------------------------------------------
// - Panorama Constructor
//----------------------------------------------------------------------------
// * texture : bitmap used as the central image being panned
// * roi : region-of-interest as a portion  of the texture map (0~1 x 0~1)
//----------------------------------------------------------------------------
Panorama::Panorama(const sf::Texture& texture, const sf::Vector2f& roi) :
    AnimatedObject(FPS),
    rate_(1),
    direction_(0),
    panX_(0),
    panY_(0),
    roi_(sf::Vector2f(1, 1)),
    image_(sf::Sprite(texture))
{
    // Panoramic textures must repeat
    const_cast<sf::Texture&>(texture).setRepeated(true);
    
    if(roi.x > 0 && roi.y > 0)
    {
        roi_.x = std::min(1.f, roi.x);
        roi_.y = std::min(1.f, roi.y);
    }

    image_.setTextureRect(sf::IntRect(0, 0, roi_.x * texture.getSize().x, roi_.y * texture.getSize().y));
    view_ = sf::View(sf::FloatRect(0, 0, texture.getSize().x, texture.getSize().y));
}

//----------------------------------------------------------------------------
// - Panorama Destructor
//----------------------------------------------------------------------------
Panorama::~Panorama()
{

}

//----------------------------------------------------------------------------
// - Get Panning Rate
//----------------------------------------------------------------------------
float Panorama::getPanningRate() const
{
    return rate_;
}

//----------------------------------------------------------------------------
// - Set Panning Rate
//----------------------------------------------------------------------------
// * rps : number of times the entire image is panned through in 1 second
//----------------------------------------------------------------------------
void Panorama::setPanningRate(float rps)
{
    if(rps > 0)
    {
        rate_ = rps;
    }
}

//----------------------------------------------------------------------------
// - Get Panning Angle
//----------------------------------------------------------------------------
float Panorama::getDirection() const
{
    return float(direction_ * 180 / M_PI);
}

//----------------------------------------------------------------------------
// - Set Panning Angle
//----------------------------------------------------------------------------
// * angle : direction (0 ~ 360 degrees) at which the panorama appears to move
//      toward, where an angle of 0 results in a right-ward pan
//----------------------------------------------------------------------------
void Panorama::setDirection(float angle)
{
    while(angle > 360)
    {
        angle -= 360;
    }

    while(angle < 0)
    {
        angle += 360;
    }

    direction_ = float(angle * M_PI / 180);
}

//----------------------------------------------------------------------------
// - Get Region of Interest
//----------------------------------------------------------------------------
const sf::Vector2f& Panorama::getROI() const
{
    return roi_;
}

//----------------------------------------------------------------------------
// - Set Region of Interest
//----------------------------------------------------------------------------
// * roi : region-of-interest as a portion  of the texture map (0~1 x 0~1)
//----------------------------------------------------------------------------
void Panorama::setROI(const sf::Vector2f& roi)
{
    if(roi.x > 0 && roi.y > 0)
    {
        roi_.x = std::min(1.f, roi.x);
        roi_.y = std::min(1.f, roi.y);
    }

    image_.setTextureRect(sf::IntRect(0, 0, roi.x * image_.getTexture()->getSize().x, roi.y * image_.getTexture()->getSize().y));\
    view_ = sf::View(sf::FloatRect(0, 0, image_.getTexture()->getSize().x, image_.getTexture()->getSize().y));
    
}

//----------------------------------------------------------------------------
// - Get Texture
//----------------------------------------------------------------------------
const sf::Texture* Panorama::getTexture() const
{
    return image_.getTexture();
}

//----------------------------------------------------------------------------
// - Set Texture
//----------------------------------------------------------------------------
// * texture : bitmap used as the central image being panned 
//----------------------------------------------------------------------------
void Panorama::setTexture(const sf::Texture& texture)
{
    // Panoramic textures must repeat
    const_cast<sf::Texture&>(texture).setRepeated(true);

    image_.setTexture(texture);
    image_.setTextureRect(sf::IntRect(0, 0, roi_.x * texture.getSize().x, roi_.y * texture.getSize().y));
    view_ = sf::View(sf::FloatRect(0, 0, image_.getTexture()->getSize().x, image_.getTexture()->getSize().y));
    
}

//----------------------------------------------------------------------------
// - Draw Panorama Overlay Image
//----------------------------------------------------------------------------
// Temporarily replaces the render window's view with a view designed to fit
// the panoramic view's region of interest, and draws it onto the window
//----------------------------------------------------------------------------
void Panorama::drawOverlays(sf::RenderTarget& target) const
{
    sf::View original = target.getView();    
    target.setView(view_);
    target.draw(image_);    
    target.setView(original);
}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Updates the panoramic view, shifting the internal drawing rectangle by a
// fraction equal to 1 / current fps of the total texture size
//----------------------------------------------------------------------------
void Panorama::step()
{
    const sf::IntRect& textureRect = image_.getTextureRect();
    panX_ -= textureRect.width * rate_ / getFPS() * cos(direction_);
    panY_ += textureRect.height * rate_ / getFPS() * sin(direction_);

    image_.setTextureRect(sf::IntRect((int)panX_, (int)panY_, textureRect.width, textureRect.height));
}
