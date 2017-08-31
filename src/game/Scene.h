#ifndef TACTICS_SCENE_H
#define TACTICS_SCENE_H

#include "../control/Cursor.h"
#include "../control/Menu.h"
#include "../control/TargetConfirmer.h"
#include "../map/Map.h"
#include "../screen/ViewEx.h"
#include "../screen/Panorama.h"
#include "../sprite/SpriteActorHUD.h"
#include "../sprite/SpriteArea.h"
#include "../settings.h"
#include "ResourceManager.h"
#include <vector>

//================================================================================
// ** Scene
//================================================================================
// Central object around which battles are based, managing the scene, stage and
// control flow
//================================================================================
class Scene : public sf::Drawable
{
// Methods
public:
    Scene(const sf::Vector2f& dimensions = sf::Vector2f(ASPECT_RATIO.x, ASPECT_RATIO.y));
    virtual ~Scene();

    void                nextTurn();
    void                endTurn();
    bool                active() const;
    void                start();
    bool                closed() const;
    void                close();

protected:
    void                setupMap();
    void                setupActors();
    void                setupStaging();
    void                setupMenus();
    void                setupControls();

    void                showHUD(SpriteActorHUD* hud, const sf::Vector3f& position);
    
    virtual void        draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
// Members - Staging
    std::vector<Actor*> actors_;
    Map*                map_;
    ViewEx              view_;
    
// Members - HUD & Menus
    sf::View            screen_;
    SpriteActorHUD*     actorHUD_;
    SpriteActorHUD*     targetHUD_;
    Menu*               battleMenu_;
    Menu*               actionMenu_;
    Menu*               mainMenu_;
    
// Members - Control Flow
    int                 acting_;
    Cursor*             cursor_;
    sf::Sprite*         cursorSprite_;
    Cursor*             moveSelector_;
    SpriteArea*         moveSelection_;
    Cursor*             targetSelector_;
    SpriteArea*         targetSelection_;
    TargetConfirmer*    targetConfirmer_;
    SpriteArea*         confirmedTargets_;
    bool                active_;
    bool                closed_;

// Members - Actor Control
    bool                moved_;
    bool                acted_;
    int                 originalFacing_;
    sf::Vector3f        originalPosition_;

// Members - Resources
    TextureManager*     textures_;
};

#endif