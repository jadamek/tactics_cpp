#include "Scene.h"
#include "../sprite/map/SpriteTile.h"
#include "InputManager.h"
#include "ActionScheduler.h"
#include <iostream>
#include <cstdlib>

//----------------------------------------------------------------------------
// - Scene Constructor
//----------------------------------------------------------------------------
Scene::Scene(const sf::Vector2f& dimensions) :
    map_(0),
    view_(sf::FloatRect(0, 0, dimensions.x, dimensions.y)),
    screen_(sf::FloatRect(0, 0, dimensions.x, dimensions.y)),
    actorHUD_(0),
    targetHUD_(0),
    battleMenu_(0),
    actionMenu_(0),
    mainMenu_(0),
    acting_(0),
    cursor_(0),
    cursorSprite_(0),
    moveSelector_(0),
    moveSelection_(0),
    targetSelector_(0),
    targetSelection_(0),
    targetConfirmer_(0),
    confirmedTargets_(0),
    active_(false),
    closed_(false),
    moved_(false),
    acted_(false),
    originalFacing_(0),
    textures_(0),
    fonts_(0),
    grid_(0),
    gridLabels_(0)
{
    view_.setCenter(0, 0);
    screen_.setCenter(0, 0);
    spot_.create(2, 2);    
}

//----------------------------------------------------------------------------
// - Scene Destructor
//----------------------------------------------------------------------------
Scene::~Scene()
{
    if(map_)                delete map_;
    if(actorHUD_)           delete actorHUD_;
    if(targetHUD_)          delete targetHUD_;
    if(battleMenu_)         delete battleMenu_;
    if(actionMenu_)         delete actionMenu_;
    if(mainMenu_)           delete mainMenu_;
    if(cursor_)             delete cursor_;
    if(cursorSprite_)       delete cursorSprite_;
    if(moveSelector_)       delete moveSelector_;
    if(moveSelection_)      delete moveSelection_;
    if(targetSelector_)     delete targetSelector_;
    if(targetConfirmer_)    delete targetConfirmer_;
    if(confirmedTargets_)   delete confirmedTargets_;
    if(textures_)           delete textures_;    
    if(fonts_)              delete fonts_;
    for(Actor* actor : actors_) delete actor;

    if(grid_)
    {
        delete [] grid_;
    }
    if(gridLabels_)
    {
        delete [] gridLabels_;
    }
}

//----------------------------------------------------------------------------
// - Start Next Turn
//----------------------------------------------------------------------------
// Executes the main sequence of events for an actor's turn, from movement to
// action to Wait
//----------------------------------------------------------------------------
void Scene::nextTurn()
{
    Actor* actor = actors_[(acting_ = ((acting_ + 1) % actors_.size()))];
    
    // Scroll to acting player
    view_.scrollTo(IsometricObject::isoToGlobal(actor->position()), 0.5);
    cursor_->goTo(sf::Vector2f(actor->position().x, actor->position().y));
    ActionScheduler::instance().schedule(Action([this, actor](){
        InputManager::instance().push(cursor_); displayBattleMenu(actor);
    }, 0.5 * FPS));    
}

//----------------------------------------------------------------------------
// - End This Turn
//----------------------------------------------------------------------------
// Resets all turn-based variables and cycles the current acting player for
// the next turn
//----------------------------------------------------------------------------
void Scene::endTurn()
{
    InputManager::instance().clear();
    moved_ = false;
    acted_ = false;

    nextTurn();
}

//----------------------------------------------------------------------------
// - Scene Is Active?
//----------------------------------------------------------------------------
bool Scene::active() const
{
    return active_;
}

//----------------------------------------------------------------------------
// - Start Scene
//----------------------------------------------------------------------------
// Sets up the scene: creates the map, actors, background and [foreground]
// staging, and the main cursor labeling the scene 'active'
//----------------------------------------------------------------------------
void Scene::start()
{
    // Initiate resource catalogs
    textures_ = new TextureManager;
    fonts_ = new FontManager;
    
    setupMap();
    setupActors();
    setupStaging();
    setupMenus();
    setupControls();

    // Set active status to begin drawing and updating
    active_ = true;

    view_.fadeIn(1);
    ActionScheduler::instance().schedule(Action([this](){nextTurn();}, FPS * 1));
}

//----------------------------------------------------------------------------
// - Scene Closed?
//----------------------------------------------------------------------------
bool Scene::closed() const
{
    return closed_;
}

//----------------------------------------------------------------------------
// - Close Scene
//----------------------------------------------------------------------------
// Frees all scene objects and clears them, labeling the scene 'closed'
//----------------------------------------------------------------------------
void Scene::close()
{
    closed_ = true;
}

//----------------------------------------------------------------------------
// - Setup Map
//----------------------------------------------------------------------------
// Creates the game map, tiles and map objects
//----------------------------------------------------------------------------
void Scene::setupMap()
{
    // Simple flat 15 x 20 map with a 1-unit layer of grasstiles, and an under
    // layer of 2-unit height dirt tiles
    map_ = new Map(15, 15);

    const sf::Texture& grass_texture = textures_->load("resources/graphics/GrassTile_32x16.png");
    const sf::Texture& dirt_texture = textures_->load("resources/graphics/DirtTile_32x16.png");

    for(int x = 0; x < map_->width(); x++)
    {
        for(int y = 0; y < map_->length(); y++)
        {
            float height = 2;

            // Dirt layer
            SpriteTile* tile_sprite = new SpriteTile(dirt_texture, MAP_SCALE.x, MAP_SCALE.y, MAP_SCALE.z * height);
            Tile* tile = new Tile(tile_sprite, height);

            map_->place(tile, x, y);

            // Grass layer
            height = 1;
            
            tile_sprite = new SpriteTile(grass_texture, MAP_SCALE.x, MAP_SCALE.y, MAP_SCALE.z * height);
            tile = new Tile(tile_sprite, height);

            map_->place(tile, x, y);
        }
    }
}

//----------------------------------------------------------------------------
// - Setup Actors
//----------------------------------------------------------------------------
// Creates all actors for this scene and places them on the map
//----------------------------------------------------------------------------
void Scene::setupActors()
{
    // Create the Assassin actor
    Actor* actor = new Actor(textures_->load("resources/graphics/Assassin.png"), map_);
    actor->setPortrait(textures_->load("resources/graphics/AssassinPortrait_64x104.png"));
    actor->setName("Assassin");
    actors_.push_back(actor);
    
    // Create the Paladin actor
    actor = new Actor(textures_->load("resources/graphics/Paladin.png"), map_);
    actor->setPortrait(textures_->load("resources/graphics/PaladinPortrait_64x104.png"));
    actors_.push_back(actor);
    actor->setName("Paladin");
    
    int x, y;

    // Randomly place all actors
    for(auto actor : actors_)
    {
        do{
            x = rand() % map_->width();
            y = rand() % map_->length();
        }while(map_->playerAt(x, y));

        actor->setPosition(sf::Vector3f(x, y, map_->height(x, y)));
        actor->face(sf::Vector2f(map_->width() / 2, map_->length() / 2));
        map_->addObject(actor);
        map_->enter(actor, x, y);
    }
}
#include <sstream>

//----------------------------------------------------------------------------
// - Setup Staging
//----------------------------------------------------------------------------
// Creates the background and foreground (LATER) for the scene
//----------------------------------------------------------------------------
void Scene::setupStaging()
{
    grid_ = new sf::RectangleShape[320];
    gridLabels_ = new sf::Text[320];

    for(int x = 0; x < 20; x++)
    {
        for(int y = 0; y < 16; y++)
        {
            grid_[x + y * 20] = sf::RectangleShape(sf::Vector2f(2, 2));
            grid_[x + y * 20].setFillColor(sf::Color::Red);
            grid_[x + y * 20].setPosition((x - 10) * 32, (y - 8) * 32);
            
            std::stringstream ss;
            ss << "(" << (x - 10) << "," << (y - 8) << ")";
            gridLabels_[x + y * 20] = sf::Text(ss.str(), fonts_->load("resources/fonts/Arial.ttf"), 8);
            gridLabels_[x + y * 20].setColor(sf::Color::Red);
            gridLabels_[x + y * 20].setPosition((x - 10) * 32 + 4, (y - 8) * 32 + 4);
        }        
    }
}

//----------------------------------------------------------------------------
// - Setup Menus
//----------------------------------------------------------------------------
// Creates the actor and target HUDs, and all menus for the scene
//----------------------------------------------------------------------------
void Scene::setupMenus()
{
    // Actor HUD - placed in the lower-left corner of the screen
    actorHUD_ = new SpriteActorHUD;
    actorHUD_->setOrigin(0, actorHUD_->getGlobalBounds().height);
    actorHUD_->setPosition(screen_.getSize().x / -2 + 16, screen_.getSize().y / 2 - 16);
    actorHUD_->hide();

    // Target HUD - placed in the lower-right corner of the screen
    targetHUD_ = new SpriteActorHUD;
    targetHUD_->setOrigin(targetHUD_->getGlobalBounds().width, targetHUD_->getGlobalBounds().height);
    targetHUD_->setPosition(screen_.getSize().x / 2 - 16, screen_.getSize().y / 2 - 16);
    targetHUD_->hide();

    // Main Menu - placed in the center of the screen
    mainMenu_ = new Menu(textures_->load("resources/graphics/MenuFrame.png"));
    mainMenu_->addOption("Quit Game", [this](){
        InputManager::instance().clear();
        view_.fadeOut(2);
        ActionScheduler::instance().schedule(Action([this](){close();}, FPS * 2));
    });
    mainMenu_->setOnCancel([this](){
        InputManager::instance().pop();
    });
    mainMenu_->setPosition(mainMenu_->getGlobalBounds().width / -2.f, mainMenu_->getGlobalBounds().height / -2.f);
    
    // Battle Menu (options filled in during turn)
    battleMenu_ = new Menu(textures_->load("resources/graphics/MenuFrame.png"));
    battleMenu_->setOnCancel(
        [this]()
        {
            if(moved_)
            {
                //view_.scrollTo(IsometricObject::isoToGlobal(originalPosition_), 0.5);
                //actors_[acting_]->setPosition(originalPosition_);
                //actors_[acting_]->face(originalFacing_);
                //moved_ = false;
                InputManager::instance().popTo(cursor_);
                // display
            }
            else{
                InputManager::instance().pop();
            }
        });
    
}

//----------------------------------------------------------------------------
// - Setup Controls
//----------------------------------------------------------------------------
// Creates the main cursor, movement and target selectors and the target
// confirmer cursor
//----------------------------------------------------------------------------
void Scene::setupControls()
{
    // Cursor sprite which is shared between all cursors/selectors
    const sf::Texture& texture = textures_->load("resources/graphics/Cursor_32x16.png");
    cursorSprite_ = new sf::Sprite(texture);
    cursorSprite_->setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

    // Main cursor object and callback actions
    cursor_ = new Cursor(*cursorSprite_, *map_);
    cursor_->setPosition(sf::Vector3f(0, 0, map_->height(0, 0)));
    map_->addObject(cursor_);

    // Scroll to the acting player and show the main battle menu
    cursor_->setOnConfirm(
        [this](const sf::Vector3f& selection)
        {
            Actor* actor = actors_[acting_];
            cursor_->setBusy(true);
            cursor_->goTo(sf::Vector2f(actor->position().x, actor->position().y));
            view_.scrollTo(IsometricObject::isoToGlobal(actor->position()), 1);
            ActionScheduler::instance().schedule(Action([this, actor](){displayBattleMenu(actor); cursor_->setBusy(false);}, 1));
        });

    // Show the HUD for any actor present in the current location, if present
    cursor_->setOnMove(
        [this](const sf::Vector3f& destination)
        {
            showHUD(actorHUD_, destination);            
        });

    // Exit the scene<!>
    cursor_->setOnCancel(
        [this]()
        {
            InputManager::instance().push(mainMenu_);
        });
}

//----------------------------------------------------------------------------
// Display Battle Menu
//----------------------------------------------------------------------------
// actor : current acting player
// Shows the main battle menu for this actor, populating the menu with the
// appropriate options and placing it at the lower-right corner of the screen
//----------------------------------------------------------------------------
void Scene::displayBattleMenu(Actor* actor)
{
    battleMenu_->clear();
    // Populate options
    if(!moved_) battleMenu_->addOption("Move", [](){std::cout << "Move." << std::endl;});
    if(!acted_) battleMenu_->addOption("Action", [](){std::cout << "Act." << std::endl;});
    battleMenu_->addOption("Wait", [this](){endTurn();});

    // Place in corner
    battleMenu_->setPosition(screen_.getSize().x / 2 - battleMenu_->getGlobalBounds().width - 16, screen_.getSize().y / 2 - battleMenu_->getGlobalBounds().height - 16);
    InputManager::instance().push(battleMenu_);
}

//----------------------------------------------------------------------------
// Show HUD
//----------------------------------------------------------------------------
// * hud : HUD object to reveal or hide if an actor is present at the position
// * position : 2-D coordinate where an actor may or may not be present
//----------------------------------------------------------------------------
void Scene::showHUD(SpriteActorHUD* hud, const sf::Vector3f& position)
{
    Actor* actor;
    
    if((actor = this->map_->playerAt(position.x, position.y)))
    {
        hud->setActor(*actor);
        hud->show();
    }
    else
    {
        hud->hide();
    }
}

//----------------------------------------------------------------------------
// - Draw Scene
//----------------------------------------------------------------------------
// Draws the scene onto a render target in the following order:
// 1) Background (panorama)
// 2) Map & Actors
// 3) HUD & Menus
// 4) Foreground (fogs, etc.)
// 5) Tint/Flash Overlays
//----------------------------------------------------------------------------
void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(active_ && !closed_)
    {        
        // Map & Actors
        target.setView(view_);
        
        if(map_)
        {
            target.draw(*map_, states);
        }

        // HUD & Menus
        target.setView(screen_);

        if(actorHUD_)
        {
            target.draw(*actorHUD_, states);
        }

        if(targetHUD_)
        {
            target.draw(*targetHUD_, states);
        }

        if(mainMenu_)
        {
            target.draw(*mainMenu_, states);
        }

        if(battleMenu_)
        {
            target.draw(*battleMenu_, states);
        }

        // Tint/Flash Overlays
        view_.drawOverlays(target);

        if(grid_){
            for(int i = 0; i < 320; i++)
            {
                target.draw(grid_[i]);
                target.draw(gridLabels_[i]);
            }
        }
    }
}
