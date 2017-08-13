#ifndef TACTICS_SPRITE_ANIMATED_H
#define TACTICS_SPRITE_ANIMATED_H

#include "../objects/AnimatedObject.h"
#include "Sprite.h"
#include "SpriteIndexed.h"
#include <map>
#include <vector>
#include <string>

//================================================================================
// ** Sprite Animated
//================================================================================
// Animated sprite which plays animations: named sequences of sprite sheet indices
//================================================================================
class SpriteAnimated : public Sprite, public AnimatedObject
{
// Methods
public:
    SpriteAnimated(SpriteIndexed* sprite = 0);
    virtual ~SpriteAnimated();

    void                        add(const std::string& name, const std::vector<int>& sequence);
    bool                        has(const std::string& name) const;
    void                        remove(const std::string& name);
    void                        play(const std::string& name = "default", bool looping = false);
    void                        playFor(const std::string& name = "default", float duration = -1);
    bool                        playing() const;
    void                        stop();
    SpriteIndexed*              getSprite();
    const SpriteIndexed*        getSprite() const;
    void                        setSprite(SpriteIndexed* sprite);
    virtual sf::FloatRect       getGlobalBounds() const;
    int                         getFramesRemaining() const;

protected:
    virtual void                draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void                step();

// Members
    int                         current_;
    std::string                 playing_;
    SpriteIndexed*              sprite_;
    int                         length_;
    bool                        loop_;
    std::map<std::string, std::vector<int>> animations_;
};

#endif