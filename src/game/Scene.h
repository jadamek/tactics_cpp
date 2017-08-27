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

    virtual void        draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
// Members - Staging
    std::vector<Actor*> actors_;
    Map*                map_;
    ViewEx              view_;
    
// Members - HUD & Menus
    sf::View            screen_;
    Menu*               battleMenu_;
    Menu*               actionMenu_;

// Members - Control Flow
    int                 acting_;
    Cursor*             cursor_;
    bool                moved_;
    bool                acted_;
    Cursor*             moveSelector_;
    SpriteArea*         moveSelection_;
    Cursor*             targetSelector_;
    SpriteArea*         targetSelection_;
    TargetConfirmer*    targetConfirmer_;
    bool                active_;
    bool                closed_;

// Members - Resources
    TextureManager*     textures_;
};

#endif