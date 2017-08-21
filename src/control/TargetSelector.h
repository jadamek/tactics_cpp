#ifndef TACTICS_TARGET_SELECTOR_H
#define TACTICS_TARGET_SELECTOR_H

#include "Cursor.h"
#include "../sprite/SpriteArea.h"
#include <vector>

//================================================================================
// ** TargetSelector
//================================================================================
// Cursor which also displays an area of interest, presumably restricting target
// selection to that area
//================================================================================
class TargetSelector : public Cursor
{
// Methods
public:
    TargetSelector(const std::vector<sf::Vector2i>& area, const sf::Texture& areaTexture,
        const sf::Color& areaColor, const sf::Texture& cursorTexture, Map* map, std::function<void()> action);
    virtual ~TargetSelector();

    bool                    contains(const sf::Vector2f& position) const;

protected:
    std::vector<SpriteArea> area_;
};

#endif