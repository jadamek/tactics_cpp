#include "SpriteMenuFrame.h"
#include <iostream>
//----------------------------------------------------------------------------
// - Sprite Menu Frame Constructor
//----------------------------------------------------------------------------
// * texture : bitmap image used for the outer frame sprites
// * size : size of the inner body of the frame
// Frame piece order (in array and expected in texture):
//      top-left
//      top-right
//      left
//      right
//      bottom-left
//      bottom-right
//      top
//      bottom
//----------------------------------------------------------------------------
SpriteMenuFrame::SpriteMenuFrame(const sf::Texture& texture, sf::Vector2u size) :
    frame_(0),
    width_x(0),
    width_y(0)
{
    if(texture.getSize().x > 4 && texture.getSize().y > 2)
    {
        if(size.x == 0)
        {
            size.x = 1;
        }
        if(size.y == 0)
        {
            size.y = 1;
        }

        frame_ = new sf::Sprite[8];
        width_x = texture.getSize().x / 4;
        width_y = texture.getSize().y / 2;

        std::cout << width_x << " x " << width_y << " on " << size.x << " x " << size.y << std::endl;

        for(int i = 0; i < 8; i++)
        {
            frame_[i].setTexture(texture);
            frame_[i].setTextureRect(sf::IntRect(width_x * (i % 4), width_y * (i / 4), width_x, width_y));
            frame_[i].setPosition(i * width_x, 0);
        }
        
        // Top-left Corner
        frame_[0].setPosition(-1 * width_x, -1 * width_y);        
        // Top-right Corner
        frame_[1].setPosition(size.x, -1 * width_y);
        // Bottom-left Corner
        frame_[4].setPosition(-1 * width_x, size.y);
        // Bottom-right Corner
        frame_[5].setPosition(size.x, size.y);
        // Left side
        frame_[2].setPosition(-1 * width_x, 0);
        frame_[2].setScale(1, size.y / float(width_y));
        // Right side
        frame_[3].setPosition(size.x, 0);
        frame_[3].setScale(1, size.y / float(width_y));
        // Top side
        frame_[6].setPosition(0, -1 * width_y);
        frame_[6].setScale(size.x / float(width_x), 1);
        // Bottom side
        frame_[7].setPosition(0, size.y);
        frame_[7].setScale(size.x / float(width_x), 1);        
    }    
}

//----------------------------------------------------------------------------
// - Sprite Menu Frame Destructor
//----------------------------------------------------------------------------
SpriteMenuFrame::~SpriteMenuFrame()
{
    if(frame_)
    {
        delete [] frame_;    
    }
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this actor's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect SpriteMenuFrame::getGlobalBounds() const
{
    if(frame_)
    {
        sf::FloatRect rect(frame_[0].getPosition(), sf::Vector2f(frame_[5].getPosition().x + width_x, frame_[5].getPosition().y + width_y));
        return getTransform().transformRect(rect);
    }
}


//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void SpriteMenuFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(frame_)
    {
        states.transform *= getTransform();
        for(int i = 0; i < 8; i++)
        {
            target.draw(frame_[i], states);
        }
    }
}