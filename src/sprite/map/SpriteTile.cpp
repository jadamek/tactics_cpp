#include <algorithm>
#include "SpriteTile.h"

//----------------------------------------------------------------------------
// - Tile Sprite Contructor
//----------------------------------------------------------------------------
// * texture : the bitmap containing the image data for the tile.
// * width : width in pixels; 0 to fit automatically to texture
// * length : width in pixels; 0 to fit automatically to texture
// * height : height in pixels
// * continuous : TRUE if the tile's sprite continues infinitely downward
//----------------------------------------------------------------------------
SpriteTile::SpriteTile(const sf::Texture& texture, float width, float length, float height, bool continuous) :
    Sprite(),
    top_(new sf::Sprite(texture)),
    body_(0),
    bottom_(0),
    width_(width),
    length_(length),
    height_(std::max(height, 0.f)),    
    continuous_(continuous)
{
    if(width <= 0 || width > texture.getSize().x / 2)
    {
        width_ = texture.getSize().x / 2;
    }

    if(length <= 0 || length > texture.getSize().y / 2)
    {
        length_ = texture.getSize().y / 2;
    }

    const_cast<sf::Texture&>(texture).setRepeated(true);

    // Top
    top_->setTextureRect(sf::IntRect(0, 0, width_, length_));
    top_->setPosition(-1 * width_ / 2, -1 * length_ / 2 - height_);

    // Body
    if(height_ > 0 || continuous_)
    {
        body_ = new sf::Sprite(texture);
        body_->setPosition(-1 * width_ / 2, -1 * height_);
        int body_height = (continuous_ ? texture.getMaximumSize() : height_);
        body_->setTextureRect(sf::IntRect(width_, 0, width_, body_height));
    }

    // Bottom
    if(!continuous_)
    {
        bottom_ = new sf::Sprite(texture);
        bottom_->setTextureRect(sf::IntRect(0, length_, width_, length_));
        bottom_->setPosition(-1 * width_ / 2, -1 * length_ / 2);        
    }
}

//----------------------------------------------------------------------------
// - Tile Sprite Destructor
//----------------------------------------------------------------------------
SpriteTile::~SpriteTile()
{
    if(body_)
    {
        delete body_;
    }
    if(bottom_)
    {
        delete bottom_;
    }
}

//----------------------------------------------------------------------------
// - Reset Height
//----------------------------------------------------------------------------            
// * height : new height of the tile sprite
// Recompute sub-sprite widths and positions for a new virtual height.
//----------------------------------------------------------------------------
void SpriteTile::resetHeight(float height)
{
    height_ = std::max(0.f, height);

    top_->setPosition(-1 * width_ / 2, -1 * length_ / 2 - height_);

    // Body
    if(height_ > 0 || continuous_)
    {
        if(!body_)
        {
            body_ = new sf::Sprite(*top_->getTexture());
        }

        body_->setPosition(-1 * width_ / 2, -1 * height_);
        int body_height = (continuous_ ? top_->getTexture()->getMaximumSize() : height_);
        body_->setTextureRect(sf::IntRect(width_, 0, width_, body_height));
    }
    else if(body_)
    {
        delete body_;
        body_ = 0;
    }
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this tile's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect SpriteTile::getGlobalBounds() const
{
    sf::FloatRect bounds(top_->getGlobalBounds());

    if(body_)
    {
        bounds.height += body_->getGlobalBounds().height;
    }

    return bounds;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void SpriteTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    if(bottom_)
    {
        target.draw(*bottom_, states);
    }

    if(body_)
    {
        target.draw(*body_, states);
    }

    target.draw(*top_, states);
}