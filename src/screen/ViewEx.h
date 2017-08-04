#ifndef TACTICS_ENHANCED_VIEW_H
#define TACTICS_ENHANCED_VIEW_H

#include <SFML/Graphics.hpp>
#include "../objects/AnimatedObject.h"
#include "../objects/IsometricObject.h"
#include "../settings.h"

namespace Shake{
    // Enumerables for enhanced functions
    enum Direction {Horizontal = 0, Vertical = 1};
}

namespace Spin{
    enum Direction {Clockwise = -1, CounterClockwise = 1};

}

//================================================================================
// ** ViewEx
//================================================================================
// Extends the SFML View to add major functionality such as zooming, scrolling,
// shaking and flashing.
//================================================================================
class ViewEx : public sf::View, public AnimatedObject
{
// Methods
public:
    ViewEx(const sf::FloatRect& viewport = sf::FloatRect(0, 0, ASPECT_RATIO.x, ASPECT_RATIO.y));
    ~ViewEx();

    // Overrides
    void                setCenter(const sf::Vector2f& center);
    void                setCenter(float x, float y);
    void                setSize(const sf::Vector2f& size);
    void                setSize(float width, float height);
    void                move(const sf::Vector2f& offset);
    void                move(float offsetX, float offsetY);
    void                setRotation(float angle);
    void                rotate(float angle);
    void                zoom(float factor);
    void                scale(float factor);
    void                reset(const sf::FloatRect& rectangle);

    // Enhanced functions - Scrolling
    void                scroll(const sf::Vector2f& offset, float duration);
    void                scrollTo(const sf::Vector2f& target, float duration);
    bool                scrolling() const;
    void                stopScrolling();
    
    // Enhanced functions - Focusing
    void                focus(const IsometricObject* object, float duration);
    bool                focusing() const;
    void                stopFocusing();

    // Enhanced functions - Shaking
    void                shake(float magnitude, float frequency, float duration = 0, Shake::Direction direction = Shake::Horizontal);
    bool                shaking() const;
    void                stopShaking();

    // Enhanced functions - Zoom
    void                zoom(float factor, float duration);
    bool                zooming() const;
    void                stopZooming();

    // Enhanced functions - Spinning
    void                spin(float rps, float revolutions = 0, Spin::Direction direction = Spin::Clockwise);
    bool                spinning() const;
    void                stopSpinning();

    // Enhanced functions - Tinting
    void                tint(const sf::Color& color, float duration);
    void                fadeIn(float duration, sf::Color start = sf::Color::Black, const sf::Color& destination = sf::Color(0, 0, 0, 0));
    void                fadeOut(float duration, sf::Color color = sf::Color::Black);
    bool                tinting() const;

    // Enhanced functions - Flashing
    void                flash(float duration, sf::Color color = sf::Color::White);
    bool                flashing() const;

    // Enhanced functions - Draw
    void                drawOverlays(sf::RenderTarget& target) const;

protected:
    void                step();

// Members
    sf::Vector2f        center_;
    float               rotation_;

    int                 flashPeak_;
    int                 flashLength_;

    sf::Color           tintColor_;
    int                 tintLength_;

    sf::RectangleShape  tintBox_;
    sf::RectangleShape  flashBox_;

    sf::Vector2f        scrollTarget_;
    int                 scrollLength_;

    const IsometricObject* focusTarget_;
    float               focusDur_;

    float               shakeAmp_;
    float               shakeFreq_;
    int                 shakePeak_;
    int                 shakeLength_;
    bool                shakeLoop_;
    Shake::Direction    shakeDir_;

    float               zoom_;
    float               zoomRate_;
    int                 zoomLength_;

    float               spinSpeed_;
    int                 spinLength_;
    bool                spinLoop_;
};

#endif