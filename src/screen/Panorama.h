#ifndef TACTICS_PANORAMA_H
#define TACTICS_PANORAMA_H

#include <SFML/Graphics.hpp>
#include "../objects/AnimatedObject.h"

//================================================================================
// ** Panorama
//================================================================================
// A panoramic view which pans along a texture image continuously in a given
// direction (angle) at a default speed of 1 full image rotation per second
//================================================================================
class Panorama : public AnimatedObject
{
// Methods
public:
    Panorama(const sf::Texture& texture, const sf::Vector2f& roi = sf::Vector2f(1, 1));
    ~Panorama();

    float               getPanningRate() const;
    void                setPanningRate(float rps);
    float               getDirection() const;
    void                setDirection(float angle);
    const sf::Vector2f& getROI() const;
    void                setROI(const sf::Vector2f& roi);
    const sf::Texture*  getTexture() const;
    void                setTexture(const sf::Texture& texture);
    void                drawOverlays(sf::RenderTarget& target) const;

protected:
    void                step();

// Members
    float               rate_;
    float               direction_;
    float               panX_;
    float               panY_;
    sf::Vector2f        roi_;
    sf::Sprite          image_;
    sf::View            view_;
};


// long it takes the sprite to pan
// through its entire texture

#endif