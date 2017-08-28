#include "Scene.h"
#include "../sprite/map/SpriteTile.h"
#include <iostream>

//----------------------------------------------------------------------------
// - Scene Constructor
//----------------------------------------------------------------------------
Scene::Scene(const sf::Vector2f& dimensions) :
    map_(0),
    view_(sf::FloatRect(0, 0, dimensions.x, dimensions.y)),
    screen_(sf::FloatRect(0, 0, dimensions.x, dimensions.y)),
    battleMenu_(0),
    actionMenu_(0),
    acting_(0),
    cursor_(0),
    moved_(false),
    acted_(false),
    moveSelector_(0),
    moveSelection_(0),
    targetSelector_(0),
    targetSelection_(0),
    targetConfirmer_(0),
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
    if(battleMenu_)         delete battleMenu_;
    if(actionMenu_)         delete actionMenu_;
    if(cursor_)             delete cursor_;
    if(moveSelector_)       delete moveSelector_;
    if(moveSelection_)      delete moveSelection_;
    if(targetSelector_)     delete targetSelector_;
    if(targetConfirmer_)    delete targetConfirmer_;
    if(textures_)           delete textures_;
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
    }
}
