#include "ViewEx.h"
#include <math.h>

# define M_PI 3.14159265358979323846

//----------------------------------------------------------------------------
// - ViewEx Constructor
//----------------------------------------------------------------------------
// * rectangle : intial viewing zone, top-left corner at x,y with view size
//       width, height
//----------------------------------------------------------------------------
ViewEx::ViewEx(const sf::FloatRect& viewport) :
    sf::View(viewport),
    AnimatedObject(FPS),
    center_(sf::View::getCenter()),
    rotation_(0),
    flashLength_(0),
    tintLength_(0),
    scrollLength_(0),
    shakeLength_(0),
    shakeLoop_(false),
    focusTarget_(0),
    zoom_(1),
    zoomLength_(0),
    spinLength_(0),
    spinLoop_(false),
    tintBox_(sf::RectangleShape(sf::Vector2f(1000, 1000))),
    flashBox_(sf::RectangleShape(sf::Vector2f(1000, 1000)))
{
    // Hide tint and flash box initially
    tintBox_.setFillColor(sf::Color(0, 0, 0, 0));
    flashBox_.setFillColor(sf::Color(0, 0, 0, 0));
}

//----------------------------------------------------------------------------
// - ViewEx Destructor
//----------------------------------------------------------------------------
ViewEx::~ViewEx()
{

}

//----------------------------------------------------------------------------
// - Set View Center (Override)
//----------------------------------------------------------------------------
// * center : a new position to center the view on
// Functionally overrides a basic center set to include updates for attached
// extensions, such as the flash color box
//----------------------------------------------------------------------------
void ViewEx::setCenter(const sf::Vector2f& center)
{
    sf::View::setCenter(center);
    center_ = center;
}

//----------------------------------------------------------------------------
// - Set View Center (Overload)
//----------------------------------------------------------------------------
void ViewEx::setCenter(float x, float y)
{
    setCenter(sf::Vector2f(x, y));
}

//----------------------------------------------------------------------------
// - Set View Window Size (Override)
//----------------------------------------------------------------------------
// * size : new width x height of the viewing window
// Functionally overrides a basic size set to include updates for extensions
//----------------------------------------------------------------------------
void ViewEx::setSize(const sf::Vector2f& size)
{
    sf::View::setSize(size * zoom_);
}

//----------------------------------------------------------------------------
// - Set View Window Size (Overload)
//----------------------------------------------------------------------------
void ViewEx::setSize(float width, float height)
{
    setSize(sf::Vector2f(width, height));
}

//----------------------------------------------------------------------------
// - Move View Center (Override)
//----------------------------------------------------------------------------
// * offset : vector describing the x,y offset to change the view's center by
//----------------------------------------------------------------------------
void ViewEx::move(const sf::Vector2f& offset)
{
    center_ += offset;
    sf::View::setCenter(getCenter() + offset);
}

//----------------------------------------------------------------------------
// - Move View Center (Overload)
//----------------------------------------------------------------------------
void ViewEx::move(float offsetX, float offsetY)
{
    move(sf::Vector2f(offsetX, offsetY));
}

//----------------------------------------------------------------------------
// - Set View Rotation Angle (Override)
//----------------------------------------------------------------------------
// * angle : angle to rotate the view by
// Functionally overrides a basic rotation set to include updates for
// extensions
//----------------------------------------------------------------------------
void ViewEx::setRotation(float angle)
{
    sf::View::setRotation(angle);
    rotation_ = angle;
}

//----------------------------------------------------------------------------
// - Rotate View (Override)
//----------------------------------------------------------------------------
// * angle : angle to rotate the view by
// Overloaded to include updates for extensions
//----------------------------------------------------------------------------
void ViewEx::rotate(float angle)
{
    sf::View::rotate(angle);
    rotation_ += angle;
}

//----------------------------------------------------------------------------
// - Scale (Zoom) Viewing Zone (Override)
//----------------------------------------------------------------------------
// * factor : scale factor to increase the window's view by.
// Extends functionality of sf::View::zoom, including updates for extensions.
//----------------------------------------------------------------------------
void ViewEx::zoom(float factor)
{
    // A zoom factor of zero or less is undefined
    if(zoom_ + factor > 0)
    {
        sf::Vector2f size = getSize() / zoom_;
        zoom_ += factor;
        size *= zoom_;

        sf::View::setSize(size);        
    }
}

//----------------------------------------------------------------------------
// - Scale (Zoom) Viewing Zone (Overload)
//----------------------------------------------------------------------------
void ViewEx::scale(float factor)
{
    zoom(factor);
}

//----------------------------------------------------------------------------
// - Reset View (Override)
//----------------------------------------------------------------------------
// * rectangle : new viewing zone, top-left corcner on x,y with region size
//       width, height
// Overloaded to include updates for extensions, returning the screen to its
// true center, rotation and 1.0 zoom factor, as well as ceasing any active
// extended actions such as spinning or shaking.
//----------------------------------------------------------------------------
void ViewEx::reset(const sf::FloatRect& rectangle)
{
    sf::View::reset(rectangle);

    // Clear any current enhanced functions other than tint/flashing
    zoom_ = 1;
    setCenter(getCenter());
    setSize(sf::Vector2f(rectangle.width, rectangle.height));
    setRotation(getRotation());
    stopScrolling();
    stopFocusing();
    stopShaking();
    stopSpinning();
    stopZooming();
}

//----------------------------------------------------------------------------
// - Scroll
//----------------------------------------------------------------------------
// * offset : the offset vector to scroll along
// * duration : time in seconds the scroll will be completed in    
// Starts the scrolling process by setting and computing a frame-wise time
// length from the duration in seconds.
//----------------------------------------------------------------------------
void ViewEx::scroll(const sf::Vector2f& offset, float duration)
{
    scrollTarget_ = center_ + offset;
    scrollLength_ = floor(duration * getFPS());
}

//----------------------------------------------------------------------------
// - Scroll to Point
//----------------------------------------------------------------------------
// * target : screen position to scroll to
// * duration : time in seconds the scroll will be completed in    
//----------------------------------------------------------------------------
void ViewEx::scrollTo(const sf::Vector2f& target, float duration)
{
    scrollTarget_ = target;
    scrollLength_ = floor(duration * getFPS());
}

//----------------------------------------------------------------------------
// - Is Scrolling?
//----------------------------------------------------------------------------
// Returns whether the view is currently scrolling
//----------------------------------------------------------------------------
bool ViewEx::scrolling() const
{
    return scrollLength_ > 0;
}

//----------------------------------------------------------------------------
// - Stop Scrolling
//----------------------------------------------------------------------------
// Ceases any scroll motion by setting the duration to 0.
//----------------------------------------------------------------------------
void ViewEx::stopScrolling()
{
    scrollLength_ = 0;
}

//----------------------------------------------------------------------------
// - Focus
//----------------------------------------------------------------------------
// * object : object to continually scroll to
// * duration : time in seconds every scroll will be completed in    
// Causes the view to always scroll to a given target if ever it is not
// currently centered on it
//----------------------------------------------------------------------------
void ViewEx::focus(const IsometricObject* object, float duration)
{
    if(duration > 0)
    {
        focusTarget_ = object;
        focusDur_ = duration;
    }
}

//----------------------------------------------------------------------------
// - Is Focusing?
//----------------------------------------------------------------------------
// Returns whether the view is currently focusing on an object
//----------------------------------------------------------------------------
bool ViewEx::focusing() const
{
    return focusTarget_ != 0;
}

//----------------------------------------------------------------------------
// - Stop Focusing
//----------------------------------------------------------------------------
// Ceases any focusing and scrolling motion by setting the target to null and
// the scroll duration to 0.
//----------------------------------------------------------------------------
void ViewEx::stopFocusing()
{
    stopScrolling();
    focusTarget_ = 0;
}

//----------------------------------------------------------------------------
// - Shake
//----------------------------------------------------------------------------
// * magnitude : amplitude of the vibrational motion in pixels
// * frequency : amplitude of the vibrational motion in periods/second
// * duration : time in seconds the shake will last. A duration less than 1 is
//       an looping (infinite) shake
// * direction : direction of the vibration; may be horizontal (0) or vertical
//       (1), and defaults to horizontal otherwise
//----------------------------------------------------------------------------
void ViewEx::shake(float magnitude, float frequency, float duration, Shake::Direction direction)
{
    if(magnitude != 0 && frequency > 0)
    {
        shakeAmp_ = magnitude;
        shakeFreq_ = frequency;

        if(duration > 0)
        {
            // Shake peak is used to implement decaying magnitude
            shakePeak_ = floor(duration * getFPS());
            shakeLength_ = shakePeak_;
        }

        shakeLoop_ = duration <= 0;
        shakeDir_ = direction;
    }
}

//----------------------------------------------------------------------------
// - Is Shaking?
//----------------------------------------------------------------------------
// Returns whether the view is currently shaking
//----------------------------------------------------------------------------
bool ViewEx::shaking() const
{
    return shakeLoop_ || shakeLength_ > 0;
}

//----------------------------------------------------------------------------
// - Stop Shaking
//----------------------------------------------------------------------------
// Ceases any shaking motion currently active, returning the screen to its
// true center
//----------------------------------------------------------------------------
void ViewEx::stopShaking()
{
    shakeLength_ = 0;
    shakeLoop_ = false;
    sf::View::setCenter(center_);
}

//----------------------------------------------------------------------------
// - Zoom
//----------------------------------------------------------------------------
// * factor : amount by which to change the view current zoom factor. A
//       factor < 0 is a zoom in, > 0 is a zoom out.
// * duration : time in seconds the gradual zoom will be complete
//----------------------------------------------------------------------------
void ViewEx::zoom(float factor, float duration)
{
    if(zoom_ + factor > 0 && duration > 0)
    {
        zoomRate_ = factor * zoom_ / duration / getFPS();
        zoomLength_ = floor(duration * getFPS());
    }
}

//----------------------------------------------------------------------------
// - Is Zooming?
//----------------------------------------------------------------------------
// Returns whether the view is currently zooming in or out
//----------------------------------------------------------------------------
bool ViewEx::zooming() const
{
    return zoomLength_ > 0;
}

//----------------------------------------------------------------------------
// - Stop Zooming
//----------------------------------------------------------------------------
// Ceases any zooming currently active
//----------------------------------------------------------------------------
void ViewEx::stopZooming()
{
    zoomLength_ = 0;
}

//----------------------------------------------------------------------------
// - Spin
//----------------------------------------------------------------------------
// * rps : the number of revolutions/second (angular speed)
// * revolutions : number of revolutions to complete. 0 is looping (inf)
// * direction : may be clockwise (-1) or counter-clockwise (1)
// Causes the view so spin at a constant rate
//----------------------------------------------------------------------------
void ViewEx::spin(float rps, float revolutions, Spin::Direction direction)
{
    if(rps > 0)
    {
        spinSpeed_ = rps * 360.f / getFPS() * direction;
        spinLength_ = floor(revolutions / rps * getFPS());
        spinLoop_ = revolutions <= 0;
    }
}

//----------------------------------------------------------------------------
// - Is Spinning?
//----------------------------------------------------------------------------
// Returns whether the view is currently spinning
//----------------------------------------------------------------------------
bool ViewEx::spinning() const
{
    return spinLength_ > 0 || spinLoop_;
}

//----------------------------------------------------------------------------
// - Stop Spinning
//----------------------------------------------------------------------------
// Ceases any spinning currently active, returning the view to its true
// rotation
//----------------------------------------------------------------------------
void ViewEx::stopSpinning()
{
    spinLength_ = 0;
    spinLoop_ = false;
    sf::View::setRotation(rotation_);
}

//----------------------------------------------------------------------------
// - Tint
//----------------------------------------------------------------------------
// * color : destination RGB tint; uses additive blending
// * duration : time in seconds to complete the tint
// Gradually transitions the screen to a target tinting color
//----------------------------------------------------------------------------
void ViewEx::tint(const sf::Color& color, float duration)
{
    if(tintBox_.getFillColor() != color && duration > 0)
    {
        tintColor_ = color;
        tintLength_ = floor(duration * getFPS());
    }
}

//----------------------------------------------------------------------------
// - Fade In
//----------------------------------------------------------------------------
// * duration : time in seconds the fade-in will last
// * source : initial color to fade-in from; made fully opaque
// * destination : destination color to fade into, also settings the tint
// Uses the screen tint functionality to mirror a fade-in from color to color
//----------------------------------------------------------------------------
void ViewEx::fadeIn(float duration, sf::Color start, const sf::Color& destination)
{
    if(duration > 0)
    {
        // Always fade-in from an opaque color
        start.a = 255;
        tintBox_.setFillColor(start);
        tint(destination, duration);
    }
}

//----------------------------------------------------------------------------
// - Fade Out
//----------------------------------------------------------------------------
// * duration : time in seconds the fade-out will last
// * color : destination color to fade out to; made fully opaque
// Uses the screen tint functionality to mirror a fade-out to color
//----------------------------------------------------------------------------
void ViewEx::fadeOut(float duration, sf::Color color)
{
    // Always fade-out to an opaque color
    color.a = 255;
    tint(color, duration);
}

//----------------------------------------------------------------------------
// - Is Tinting
//----------------------------------------------------------------------------
// Returns whether the screen tint is currently transitioning
//----------------------------------------------------------------------------
bool ViewEx::tinting() const
{
    return tintLength_ > 0;
}

//----------------------------------------------------------------------------
// - Flash
//----------------------------------------------------------------------------
// * duration : time in seconds the flashing will last, reaching peak at 1/2 t
// * color : RGB color to flash on screen, from 0 alpha to 255 at peak, then
//       back to zero
//----------------------------------------------------------------------------
void ViewEx::flash(float duration, sf::Color color)
{
    if(duration > 0)
    {
        color.a = 0;
        flashBox_.setFillColor(color);
        flashLength_ = floor(duration * getFPS());

        // Flash reaches highest opacity at the center of its duration
        flashPeak_ = ceil(flashLength_ / 2.f);
    }
}

//----------------------------------------------------------------------------
// - Is Flashing
//----------------------------------------------------------------------------
// Returns whether the screen is currently flashing
//----------------------------------------------------------------------------
bool ViewEx::flashing() const
{
    return flashLength_ > 0;
}

//----------------------------------------------------------------------------
// - Draw View Overlays
//----------------------------------------------------------------------------
// Temporarily replaces the render window's view with a view designed to fit
// the tint and flash screens, and draws them on the window
//----------------------------------------------------------------------------
void ViewEx::drawOverlays(sf::RenderTarget& target) const
{
    sf::View original = target.getView();
    target.setView(sf::View());

    target.draw(tintBox_);
    if(flashLength_ > 0)
    {
        target.draw(flashBox_);
    }

    target.setView(original);
}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Updates all active screen actions, such as flashing and shaking
//----------------------------------------------------------------------------
void ViewEx::step()
{
    // Update Focusing
    if((focusTarget_ && !scrolling()) && center_ != focusTarget_->getGlobalPosition())
    {
        scrollTo(focusTarget_->getGlobalPosition(), focusDur_);
    }

    // Update Scrolling
    if(scrollLength_ > 0)
    {
        center_ += (scrollTarget_ - center_) / (float)scrollLength_--;
        sf::View::setCenter(center_);
    }

    // Update Shaking
    if(shakeLength_ > 0 || shakeLoop_)
    {
        float wave = shakeAmp_ * sin(2 * M_PI / getFPS() * shakeFreq_ * shakeLength_--);

        // Attenuate if finite shake length
        if(!shakeLoop_)
        {
            wave *= shakeLength_ / shakePeak_;
        }

        sf::View::setCenter(center_ + sf::Vector2f(wave * (1 - shakeDir_), wave * shakeDir_));
    }

    // Update Zooming
    if(zoomLength_ > 0)
    {
        sf::Vector2f size = getSize() / zoom_;
        zoom_ += zoomRate_;
        size *= zoom_;

        sf::View::setSize(size);
        zoomLength_--;
    }

    // Update Spinning
    if(spinLength_ > 0 || spinLoop_)
    {
        sf::View::rotate(spinSpeed_);

        if(!spinLoop_)
        {
            if(--spinLength_ <= 0)
            {
                stopSpinning();
            }
        }
    }

    // Update Tinting
    if(tintLength_ > 0)
    {
        sf::Color increment = tintBox_.getFillColor();
        increment.r += (tintColor_.r - increment.r) / tintLength_;
        increment.g += (tintColor_.g - increment.g) / tintLength_;
        increment.b += (tintColor_.b - increment.b) / tintLength_;
        increment.a += (tintColor_.a - increment.a) / tintLength_;
        tintBox_.setFillColor(increment);
        tintLength_--;
    }

    // Update Flashing
    if(flashLength_ > 0)
    {
        sf::Color flash = flashBox_.getFillColor();
        flash.a = (255.0 * (1 - abs((float)flashLength_ - flashPeak_) / flashPeak_));

        flashBox_.setFillColor(flash);
        flashLength_--;
    }
}