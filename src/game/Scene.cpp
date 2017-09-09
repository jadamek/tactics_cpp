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
    targetSelector_(0),
    targetConfirmer_(0),
    highlightArea_(0),
    active_(false),
    closed_(false),
    moved_(false),
    acted_(false),
    confirmedMove_(false),
    originalFacing_(0),
    textures_(0),
    fonts_(0),
    grid_(0),
    gridLabels_(0)
{
    view_.setCenter(0, 0);
    screen_.setCenter(0, 0);
    spot_.create(2, 2);  

    setup();
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
    if(targetSelector_)     delete targetSelector_;
    if(targetConfirmer_)    delete targetConfirmer_;
    if(highlightArea_)      delete highlightArea_;
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
    originalPosition_ = actor->position();
    originalFacing_ = actor->facing();
    
    // Add the main cursor to the bottom of the input stack
    InputManager::instance().push(cursor_);

    // Scroll to acting player
    displayBattleMenu(actor);
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
    confirmedMove_ = false;

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
// - Setup Scene
//----------------------------------------------------------------------------
// Runs all initial object creation and setup required for the scene, such as
// initializing the map, actors, menus and cursors
//----------------------------------------------------------------------------
void Scene::setup()
{    
    // Initiate resource catalogs
    textures_ = new TextureManager;
    fonts_ = new FontManager;  

    setupMap();
    setupActors();
    setupStaging();

    // Menus
    setupMainMenu();
    setupBattleMenu();
    setupActionMenu();
    setupHUDs();

    // Cursor sprite shared by all battle scene cursors
    const sf::Texture& texture = textures_->load("resources/graphics/Cursor_32x16.png");
    cursorSprite_ = new sf::Sprite(texture);
    cursorSprite_->setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);

    // Controls
    setupCursor();
    setupMoveSelector();
    setupTargetSelector();
    setupTargetConfirmer();
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
    for(int a = 0; a < actors_.size(); a++)
    {
        do{
            x = rand() % map_->width() / 3;
            y = rand() % map_->length() / 3;
        }while(map_->playerAt(x, y));

        actors_[a]->setPosition(sf::Vector3f(x, y, map_->height(x, y)));
        actors_[a]->face(sf::Vector2f(map_->width() / 2, map_->length() / 2));
        actors_[a]->setSpeed(3);
        map_->addObject(actors_[a]);
        map_->enter(actors_[a], x, y);
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
// - Setup Main Menu
//----------------------------------------------------------------------------
// Creates the main menu, which offers game settings, battle suspend, etc.
//----------------------------------------------------------------------------
void Scene::setupMainMenu()
{
    mainMenu_ = new Menu(textures_->load("resources/graphics/MenuFrame.png"));

    mainMenu_->addOption("Quit Game", [this](){
        InputManager::instance().clear();
        view_.fadeOut(2);
        ActionScheduler::instance().schedule(Action([this](){close();}, FPS * 2));
    });

    mainMenu_->setOnCancel([this](){
        InputManager::instance().pop();
    });

    // Place menu in the center of the screen    
    mainMenu_->setPosition(mainMenu_->getGlobalBounds().width / -2.f, mainMenu_->getGlobalBounds().height / -2.f);
}

//----------------------------------------------------------------------------
// - Setup Battle Menu
//----------------------------------------------------------------------------
// Create the main battle menu, whose combat options are determined during the
// turn dynamically
//----------------------------------------------------------------------------
void Scene::setupBattleMenu()
{
    battleMenu_ = new Menu(textures_->load("resources/graphics/MenuFrame.png"));

    battleMenu_->setOnCancel([this]()
    {
        // Cancel the users last movement action, returning to their original placement
        // and facing at the turn's start if they have moved without acting
        if(moved_ && !confirmedMove_)
        {
            cancelMove();

            // Pop intermediate input controllers (movement selector, etc) 
            InputManager::instance().popTo(cursor_);

            displayBattleMenu(actors_[acting_]);
        }
        else{
            InputManager::instance().pop();
        }
    });
}

//----------------------------------------------------------------------------
// - Setup Action Menu
//----------------------------------------------------------------------------
// Create the action/skill menu, whose options are determined dynamically by
// the acting player during the turn
//----------------------------------------------------------------------------
void Scene::setupActionMenu()
{
    actionMenu_ = new Menu(textures_->load("resources/graphics/MenuFrame.png"));

    actionMenu_->setOnCancel([this]()
    {
        InputManager::instance().pop();            
    });
}

//----------------------------------------------------------------------------
// - Setup HUDs
//----------------------------------------------------------------------------
// Creates the actor and target HUDs, which display the currently focused
// player and the target of a skill, respectively
//----------------------------------------------------------------------------
void Scene::setupHUDs()
{
    // Actor HUD - placed in the lower-left corner of the screen
    actorHUD_ = new SpriteActorHUD;
    actorHUD_->setOrigin(0, actorHUD_->getGlobalBounds().height);
    actorHUD_->setPosition(screen_.getSize().x / -2 + 16, screen_.getSize().y / 2 - 16);

    // Target HUD - placed in the lower-right corner of the screen
    targetHUD_ = new SpriteActorHUD;
    targetHUD_->setOrigin(targetHUD_->getGlobalBounds().width, targetHUD_->getGlobalBounds().height);
    targetHUD_->setPosition(screen_.getSize().x / 2 - 16, screen_.getSize().y / 2 - 16);
}

//----------------------------------------------------------------------------
// - Setup Main Cursor
//----------------------------------------------------------------------------
// Creates the main cursor, and defines its callbacks
//----------------------------------------------------------------------------
void Scene::setupCursor()
{    
    cursor_ = new Cursor(*cursorSprite_, *map_);
    cursor_->setPosition(sf::Vector3f(0, 0, map_->height(0, 0)));
    map_->addObject(cursor_);

    // Scroll to the acting player and show the battle menu
    cursor_->setOnConfirm([this](const sf::Vector3f& selection){
        displayBattleMenu(actors_[acting_]);
    });

    // Display the HUD for any present actor in the current space
    cursor_->setOnMove([this](const sf::Vector3f& destination){
        actorHUD_->setActor(map_->playerAt(destination.x, destination.y));
    });

    // Open the main menu
    cursor_->setOnCancel([this](){
        InputManager::instance().push(mainMenu_);
    });
}

//----------------------------------------------------------------------------
// - Setup Move Destination Cursor
//----------------------------------------------------------------------------
// Creates the cursor responsible for selecting the actor's intended movement
//----------------------------------------------------------------------------
void Scene::setupMoveSelector()
{
    moveSelector_ = new Cursor(*cursorSprite_, *map_);
    map_->addObject(moveSelector_);

    // Select current position to have the actor move there
    moveSelector_->setOnConfirm([this](const sf::Vector3f& selection){
        if(highlightArea_->contains(sf::Vector2f(selection.x, selection.y)))
        {
            moved_ = true;
            move(actors_[acting_], selection);            
        }
        else
        {
            // play 'Nope' sound
        }
    });

    // Display the HUD for any present actor in the current space
    moveSelector_->setOnMove([this](const sf::Vector3f& destination){
        actorHUD_->setActor(map_->playerAt(destination.x, destination.y));
    });

    // Return to the battle menu
    moveSelector_->setOnCancel([this](){
        clearHighlight();
        InputManager::instance().pop();
    });
}

//----------------------------------------------------------------------------
// - Setup Skill Target Cursor
//----------------------------------------------------------------------------
// Creates the cursor responsible for selecting an actor's target position for
// casting a skill
//----------------------------------------------------------------------------
void Scene::setupTargetSelector()
{
    targetSelector_ = new Cursor(*cursorSprite_, *map_);
    map_->addObject(targetSelector_);

    // Display the HUD for any present actor in the current space
    targetSelector_->setOnMove([this](const sf::Vector3f& destination){
        actorHUD_->setActor(map_->playerAt(destination.x, destination.y));
    });

    // Return to the action menu
    targetSelector_->setOnCancel([this](){
        clearHighlight();
        InputManager::instance().pop();
    });
}

//----------------------------------------------------------------------------
// - Setup Target Confirmation Cursor
//----------------------------------------------------------------------------
// Creats the cursor which cycles through any affected actors during a skill
// cast, displaying the target HUD complete with some battle preview stats
//----------------------------------------------------------------------------
void Scene::setupTargetConfirmer()
{
    targetConfirmer_ = new TargetConfirmer(*cursorSprite_);
    map_->addObject(targetConfirmer_);

    // Display the target HUD (battle preview) for the current target actor
    targetConfirmer_->setOnMove([this](Actor* target){
        view_.scrollTo(IsometricObject::isoToGlobal(target->position()), 0.2);
        targetHUD_->setActor(target);
    });
}

//----------------------------------------------------------------------------
// Display Battle Menu
//----------------------------------------------------------------------------
// * actor : current acting player
// Shows the main battle menu for this actor, populating the menu with the
// appropriate options and placing it at the lower-right corner of the screen
//----------------------------------------------------------------------------
void Scene::displayBattleMenu(Actor* actor)
{
    // Reset all cursor positions to the current acting player
    cursor_->setPosition(actor->position());
    moveSelector_->setPosition(actor->position());
    targetSelector_->setPosition(actor->position());
    
    battleMenu_->clear();

    // Create Battle Menu options for this actor    
    if(!moved_) battleMenu_->addOption("Move", [this, actor](){selectDestination(actor);});
    if(!acted_) battleMenu_->addOption("Action", [this, actor](){displayActionMenu(actor);});
    battleMenu_->addOption("Wait", [this](){endTurn();});
    
    // Place in corner
    battleMenu_->setPosition(screen_.getSize().x / 2 - battleMenu_->getGlobalBounds().width - 16, screen_.getSize().y / 2 - battleMenu_->getGlobalBounds().height - 16);

    // Scroll to acting player
    view_.scrollTo(IsometricObject::isoToGlobal(actor->position()), 0.5);

    // After scrolling, show the HUD for the actor and the battle menu
    ActionScheduler::instance().schedule(Action([this, actor]()
    {
        actorHUD_->setActor(actor);
        InputManager::instance().push(battleMenu_);        
    }, 0.5 * FPS));
}

//----------------------------------------------------------------------------
// Display Action Menu
//----------------------------------------------------------------------------
// * actor : current acting player
// Shows the action menu for this actor, populating the menu with the
// available skills the actor has. Places menu at at the lower-right corner
//----------------------------------------------------------------------------
void Scene::displayActionMenu(Actor* actor)
{
    actionMenu_->clear();

    // Populate menu with actor's learned skills, lead by a basic Attack
    for(int s = 0; s < actor->getSkills().size(); s++)
    {
        actionMenu_->addOption(actor->getSkills()[s]->name(),[this, actor, s](){
            selectTargets(actor->getSkills()[s]);
        });
    }

    // Place in lower-right corner
    actionMenu_->setPosition(screen_.getSize().x / 2 - actionMenu_->getGlobalBounds().width - 16, screen_.getSize().y / 2 - actionMenu_->getGlobalBounds().height - 16);

    InputManager::instance().push(actionMenu_);
}

//----------------------------------------------------------------------------
// Select Move Destination
//----------------------------------------------------------------------------
// * actor : current acting player
// Displays an area depicting all positions where the user can move in blue,
// as well as a cursor to select one of those positions
//----------------------------------------------------------------------------
void Scene::selectDestination(Actor* actor)
{
    // Highlight all possible movements
    highlight(actor->reach(), sf::Color(120, 120, 255));

    InputManager::instance().push(moveSelector_);
}

//----------------------------------------------------------------------------
// Select Skill Target
//----------------------------------------------------------------------------
// * skill : chosen skill to cast
// Displays an area depicting all positions where the user can cast the skill
// in red, as well as a cursor to select one of those positions
//----------------------------------------------------------------------------
void Scene::selectTargets(Skill* skill)
{
    // Highlight all possible cast targets
    highlight(skill->range(), sf::Color(255, 120, 120));
    
    // Confirm target position and go to display affected players
    targetSelector_->setOnConfirm([this, skill](const sf::Vector3f& selection)
    {
        if(highlightArea_->contains(sf::Vector2f(selection.x, selection.y)) && skill->effective(selection))
        {
            InputManager::instance().pop();
            confirmTargets(skill, selection);
        }
        else
        {
            // Make a 'nope' noise
        }           
    });

    InputManager::instance().push(targetSelector_);       
}

//----------------------------------------------------------------------------
// Confirm Target Selection
//----------------------------------------------------------------------------
// * skill : chosen skill to cast
// * target : position chosen as the center of the cast
// Displays red squares under all affected players, as well as a cursor that
// that cycles through each player
//----------------------------------------------------------------------------
void Scene::confirmTargets(Skill* skill, const sf::Vector3f& target)
{   
    // Set targets to all those affected by the skill when cast at the given
    // source position 
    auto targets = skill->affected(target);

    targetConfirmer_->setTargets(targets);

    // Scroll view to first victim
    view_.scrollTo(IsometricObject::isoToGlobal(targetConfirmer_->position()), 0.2);

    // Highlight space under all chosen cast target actors, and the actors themselves
    std::vector<sf::Vector2f> area;
    for(int a = 0; a < targets.size(); a++)
    {
        targets[a]->focus();
        area.push_back(sf::Vector2f(targets[a]->position().x, targets[a]->position().y));
    }

    highlight(area, sf::Color(255, 120, 120));

    // Show the caster in the player HUD
    actorHUD_->setActor(skill->caster());
    
    // Show target HUD (battle preview) for first victim
    targetHUD_->setActor(targets[0]);

    // Confirm skill cast!!!
    targetConfirmer_->setOnConfirm([this, skill, targets]()
    {
        cast(skill, targets); 
    });
    
    // Return to target selection and remove all targetting/highlighting effects
    targetConfirmer_->setOnCancel([this, skill, targets](){
        targetHUD_->clear();
        clearHighlight();
        for(int a = 0; a < targets.size(); a++)
        {
            targets[a]->unfocus();
        }

        InputManager::instance().pop();
        selectTargets(skill);
    });

    InputManager::instance().push(targetConfirmer_);
}

//----------------------------------------------------------------------------
// - Display Area Sprites
//----------------------------------------------------------------------------
// * area : list of positions to draw a highlight square sprite on
// * color : color of highlight squares to be drawn
//----------------------------------------------------------------------------
void Scene::highlight(const std::vector<sf::Vector2f>& area, const sf::Color& color)
{
    // Remove any old highlighting
    clearHighlight();

    highlightArea_ = new SpriteArea(textures_->load("resources/graphics/AreaSquare.png"), area, *map_, color);
    map_->addObject(highlightArea_);
}

//----------------------------------------------------------------------------
// - Remove Area Highlighting
//----------------------------------------------------------------------------
void Scene::clearHighlight()
{
    if(highlightArea_ != 0)
    {
        delete highlightArea_;
        highlightArea_ = 0;
    }
}

//----------------------------------------------------------------------------
// - Move Actor
//----------------------------------------------------------------------------
void Scene::move(Actor* actor, const sf::Vector3f& destination)
{
    // Disable input during move
    InputManager::instance().getHandler()->setActive(false);

    // Highlight destination
    highlight(std::vector<sf::Vector2f>(1, sf::Vector2f(destination.x, destination.y)), sf::Color(120, 120, 255));

    // Focus the view on the moving actor
    view_.focus(actor);

    // Play actor's "walking" animation
    actor->getSprite()->play("walk", true);

    // Send them walking to their destination along the shortest path there
    actor->walkAlong(actor->shortestPath(sf::Vector2f(destination.x, destination.y)));

    // Change the actor's position record in the map
    map_->exit(originalPosition_.x, originalPosition_.y);
    map_->enter(actor, destination.x, destination.y);

    // When the actor is finished moving, unfocus the view, remove the highlighting,
    // return the actor to its default animation and display the main battle menu
    Action action([this, actor, &destination]()
    {
        view_.stopFocusing();
        clearHighlight();
        actor->getSprite()->play("default", true);
        displayBattleMenu(actor);
    }, -1);
    action.setTrigger([actor](){return !actor->walking();});

    ActionScheduler::instance().schedule(action);
}

//----------------------------------------------------------------------------
// - Revert Last Movement
//----------------------------------------------------------------------------
void Scene::cancelMove()
{
    Actor* actor = actors_[acting_];
    
    // Undo changes to map positioning record
    map_->exit(actor->position().x, actor->position().y);
    map_->enter(actor, originalPosition_.x, originalPosition_.y);

    // Return the player to their original position and facing
    actor->setPosition(originalPosition_);
    actor->face(originalFacing_);

    moved_ = false;
}

//----------------------------------------------------------------------------
// - Cast Skill
//----------------------------------------------------------------------------
// * skill : skill being cast
// * targets : list of all actors this skill will target
//----------------------------------------------------------------------------
void Scene::cast(Skill* skill, const std::vector<Actor*>& targets)
{    
    // Block all input
    InputManager::instance().getHandler()->setActive(false);

    // Hide all HUDs
    actorHUD_->clear();
    targetHUD_->clear();

    // Remove map highlighting
    clearHighlight();

    // Remove any latent actor highlighting from all victims
    for(int a = 0; a < targets.size(); a++)
    {
        targets[a]->unfocus();
    }

    // If the player has already moved, they can no longer cancel their movement
    confirmedMove_ = moved_;
    
    acted_ = true;

    // When the actor is finished casting this skill, display the battle menu
    Action action([this, skill]()
    {
        // Remove any intermediate controllers (action menu)
        InputManager::instance().popTo(cursor_);
        displayBattleMenu(skill->caster());
    }, -1);
    action.setTrigger([skill](){return !skill->casting();});

    // Cast the skill!
    skill->use(targets);

    ActionScheduler::instance().schedule(action);    
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

        if(actionMenu_)
        {
            target.draw(*actionMenu_, states);
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
