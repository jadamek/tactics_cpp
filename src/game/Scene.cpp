#include "Scene.h"
#include "../sprite/map/SpriteTile.h"
#include "InputManager.h"
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
    battleMenu_(0),
    actionMenu_(0),
    acting_(0),
    cursor_(0),
    cursorSprite_(0),
    moved_(false),
    acted_(false),
    moveSelector_(0),
    moveSelection_(0),
    targetSelector_(0),
    targetSelection_(0),
    targetConfirmer_(0),
    confirmedTargets_(0),
    active_(false),
    closed_(false),
    textures_(0)
{
    view_.setCenter(0, 0);
    screen_.setCenter(0, 0);
}

//----------------------------------------------------------------------------
// - Scene Destructor
//----------------------------------------------------------------------------
Scene::~Scene()
{
    if(map_)                delete map_;
    if(actorHUD_)           delete actorHUD_;
    if(battleMenu_)         delete battleMenu_;
    if(actionMenu_)         delete actionMenu_;
    if(cursor_)             delete cursor_;
    if(cursorSprite_)       delete cursorSprite_;
    if(moveSelector_)       delete moveSelector_;
    if(moveSelection_)      delete moveSelection_;
    if(targetSelector_)     delete targetSelector_;
    if(targetConfirmer_)    delete targetConfirmer_;
    if(confirmedTargets_)   delete confirmedTargets_;
    if(textures_)           delete textures_;    
    for(Actor* actor : actors_) delete actor;
}

//----------------------------------------------------------------------------
// - Start Next Turn
//----------------------------------------------------------------------------
// Executes the main sequence of events for an actor's turn, from movement to
// action to Wait
//----------------------------------------------------------------------------
void Scene::nextTurn()
{

}

//----------------------------------------------------------------------------
// - End This Turn
//----------------------------------------------------------------------------
// Resets all turn-based variables and cycles the current acting player for
// the next turn
//----------------------------------------------------------------------------
void Scene::endTurn()
{

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

    setupMap();
    setupActors();
    setupStaging();
    setupControls();

    InputManager::instance().push(cursor_);
    
    // Set active status to begin drawing and updating
    active_ = true;
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

//----------------------------------------------------------------------------
// - Setup Staging
//----------------------------------------------------------------------------
// Creates the background and foreground (LATER) for the scene
//----------------------------------------------------------------------------
void Scene::setupStaging()
{

}

//----------------------------------------------------------------------------
// - Setup Controls
//----------------------------------------------------------------------------
// Creates the main cursor and actor HUD for the scene
//----------------------------------------------------------------------------
void Scene::setupControls()
{
    // Actor HUD - placed in the lower-left corner of the screen
    actorHUD_ = new SpriteActorHUD;
    actorHUD_->setOrigin(0, actorHUD_->getGlobalBounds().height);
    actorHUD_->setPosition(screen_.getSize().x / -2, screen_.getSize().y / 2);
    actorHUD_->hide();

    // Cursor sprite which is shared between all cursors/selectors
    const sf::Texture& texture = textures_->load("resources/graphics/Cursor_32x16.png");
    cursorSprite_ = new sf::Sprite(texture);
    cursorSprite_->setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

    // Main cursor object and callback actions
    cursor_ = new Cursor(*cursorSprite_, *map_);
    cursor_->setPosition(sf::Vector3f(0, 0, map_->height(0, 0)));
    map_->addObject(cursor_);

    // Show the HUD for any actor present in the current location, if present
    cursor_->setOnMove(
        [this](const sf::Vector3f& destination)
        {
            Actor* actor;

            if((actor = this->map_->playerAt(destination.x, destination.y)))
            {
                actorHUD_->setActor(*actor);
                actorHUD_->show();
            }
            else
            {
                actorHUD_->hide();
            }
        });

    // Exit the scene<!>
    cursor_->setOnCancel(
        [this]()
        {
            this->close();
        });
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
    }
}
