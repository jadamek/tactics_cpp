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
    virtual void        start();
    bool                closed() const;
    void                close();

protected:
// Methods - Setup 
    virtual void        setup();
    void                setupMap();
    void                setupActors();
    void                setupStaging();

// Methods - Setup Menus and UIs
    void                setupMainMenu();
    void                setupBattleMenu();
    void                setupActionMenu();
    void                setupHUDs();

// Methods - Setup Cursors and Controllers
    void                setupCursor();
    void                setupMoveSelector();
    void                setupTargetSelector();
    void                setupTargetConfirmer();

// Methods - Menu/Control Displays
    void                displayBattleMenu(Actor* actor);
    void                displayActionMenu(Actor* actor);
    void                selectDestination(Actor* actor);
    void                selectTargets(Skill* skill);
    void                confirmTargets(Skill* skill, const sf::Vector3f& target);

// Methods - Object Controls
    void                highlight(const std::vector<sf::Vector2f>& area, const sf::Color& color);
    void                clearHighlight();
    void                move(Actor* actor, const sf::Vector3f& destination);
    void                cancelMove();
    void                cast(Skill* skill, const std::vector<Actor*>& targets);
    
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
    bool                active_;
    bool                closed_;
    sf::Sprite*         cursorSprite_;
    Cursor*             cursor_;
    Cursor*             moveSelector_;
    Cursor*             targetSelector_;
    TargetConfirmer*    targetConfirmer_;
    SpriteArea*         highlightArea_;

// Members - Actor Control
    bool                moved_;
    bool                acted_;
    bool                confirmedMove_;
    int                 originalFacing_;
    sf::Vector3f        originalPosition_;

// Members - Resources
    TextureManager*     textures_;
    FontManager*        fonts_;

    sf::Texture         spot_;
    sf::RectangleShape* grid_;
    sf::Text*           gridLabels_;
};

#endif