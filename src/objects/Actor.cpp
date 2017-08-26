#include "Actor.h"
#include <math.h>
#include <queue>

# define M_PI 3.14159265358979323846

//----------------------------------------------------------------------------
// - Actor Constructor
//----------------------------------------------------------------------------
// * texture : bitmap for this actor's directed sprite animations
//----------------------------------------------------------------------------
Actor::Actor(const sf::Texture& texture, const Map* ground) :
    MobileObject(ground),
    sprite_(0),
    baseSprite_(new SpriteDirected(texture, 20, 32)),
    portrait_(0),
    name_("Combatant")
{
    baseSprite_->setOrigin(10, 28);
    baseSprite_->setPosition(0, 0);
    sprite_ = new SpriteAnimated(baseSprite_);
    sprite_->play("default", true);        
}

//----------------------------------------------------------------------------
// - Actor Destructor
//----------------------------------------------------------------------------
Actor::~Actor()
{
    delete sprite_;
    if(portrait_)
    {
        delete portrait_;
    }
}

//----------------------------------------------------------------------------
// - Walk A Relative Distance
//----------------------------------------------------------------------------
// * distance : relative position the actor will eventually arrive at to their
//          current one
// Sets the actor walking toward a single relative location
//----------------------------------------------------------------------------
void Actor::walk(const sf::Vector2f& distance)
{
    walkTo(sf::Vector2f(position().x + distance.x, position().y + distance.y));
}

//----------------------------------------------------------------------------
// - Walk To Single Position
//----------------------------------------------------------------------------
// * position : position the actor will eventually arrive at
// Sets the actor walking toward a single location
//----------------------------------------------------------------------------
void Actor::walkTo(const sf::Vector2f& position)
{
    face(position);
    moveTo(sf::Vector3f(position.x, position.y, 0));
}

//----------------------------------------------------------------------------
// - Move Along a Path
//----------------------------------------------------------------------------
// * path : a list of consecutive destinations
// Sets the actor's destination as a path, or list of consecutive locations to
// walk to
//----------------------------------------------------------------------------
void Actor::walkAlong(const std::list<sf::Vector2f>& path)
{
    path_ = path;
    walkTo(path_.front());    
    path_.pop_front();
}

//----------------------------------------------------------------------------
// - Is Actor Walking?
//----------------------------------------------------------------------------
bool Actor::walking() const
{
    return moving() || !path_.empty();
}

//----------------------------------------------------------------------------
// - Stop Walking
//----------------------------------------------------------------------------
// Cancels current destination and motion
//----------------------------------------------------------------------------
void Actor::stopWalking()
{
    stopMoving();
    path_.clear();
}

//----------------------------------------------------------------------------
// - Structure for BFS Search during shortest path and reach computation
//----------------------------------------------------------------------------
struct BFSToken{
    sf::Vector2f position;
    int distance;
};

//----------------------------------------------------------------------------
// - Compute Reach
//----------------------------------------------------------------------------
// Returns a vector of all reachable positions by this unit's movement using
// breadth-first search guided by passability laws
//----------------------------------------------------------------------------
std::vector<sf::Vector2f> Actor::reach() const
{
    // Replace later (MovementStyle?)
    int move = 6;
    
    std::vector<sf::Vector2f> area;
    sf::Vector2f origin = sf::Vector2f(position().x, position().y) - sf::Vector2f(move, move);
    int width = 2 * move + 1;
    std::vector<bool> visited(width * width, false);
    std::queue<BFSToken> queue;

    queue.push(BFSToken{sf::Vector2f(position().x, position().y), move});
    visited[move * (1 +  width)] = true;

    // Loop variables    
    BFSToken token;
    sf::Vector2f adjacent;
    int v_i;

    while(!queue.empty())
    {
        token = queue.front();
        queue.pop();

        if(occupiable(token.position))
        {
            area.push_back(token.position);
        }

        if(token.distance > 0)
        {
            // Check Rightward Position
            adjacent = sf::Vector2f(token.position.x + 1, token.position.y);
            v_i = int(adjacent.x - origin.x) + int(adjacent.y - origin.y) * width;
            if((adjacent.x >= 0 && adjacent.y >= 0))
            {
                if(!visited[v_i] && passable(token.position, adjacent)){
                    visited[v_i] = true;
                    queue.push(BFSToken{adjacent, token.distance - 1});
                }
            }
            
            // Check Leftward Position
            adjacent = sf::Vector2f(token.position.x - 1, token.position.y);
            v_i = int(adjacent.x - origin.x) + int(adjacent.y - origin.y) * width;
            if((adjacent.x >= 0 && adjacent.y >= 0))
            {
                if(!visited[v_i] && passable(token.position, adjacent)){
                    visited[v_i] = true;
                    queue.push(BFSToken{adjacent, token.distance - 1});
                }
            }
    
            // Check Downward Position
            adjacent = sf::Vector2f(token.position.x, token.position.y + 1);
            v_i = int(adjacent.x - origin.x) + int(adjacent.y - origin.y) * width;
            if((adjacent.x >= 0 && adjacent.y >= 0))
            {
                if(!visited[v_i] && passable(token.position, adjacent)){
                    visited[v_i] = true;
                    queue.push(BFSToken{adjacent, token.distance - 1});
                }
            }
    
            // Check Upward Position
            adjacent = sf::Vector2f(token.position.x, token.position.y - 1);
            v_i = int(adjacent.x - origin.x) + int(adjacent.y - origin.y) * width;
            if((adjacent.x >= 0 && adjacent.y >= 0))
            {
                if(!visited[v_i] && passable(token.position, adjacent)){
                    visited[v_i] = true;
                    queue.push(BFSToken{adjacent, token.distance - 1});
                }
            }
        }        
    }

    return area;
}

//----------------------------------------------------------------------------
// - Get Height (Override)
//----------------------------------------------------------------------------
// * position : (x,y) position relative to the center of the actor
//----------------------------------------------------------------------------
float Actor::getHeight(const sf::Vector2f& position) const
{
    return 3;
}

//----------------------------------------------------------------------------
// - Get Global Bounding Rectangle (Override)
//----------------------------------------------------------------------------
// Returns the rectangle this actor's sprite inscribes
//----------------------------------------------------------------------------
sf::FloatRect Actor::getGlobalBounds() const
{
    return sprite_->getGlobalBounds();        
}

//----------------------------------------------------------------------------
// - Set Texture
//----------------------------------------------------------------------------
// * texture : bitmap for this actor's directed sprite animations
//----------------------------------------------------------------------------
void Actor::setTexture(const sf::Texture& texture)
{
    baseSprite_->setTexture(texture);
}

//----------------------------------------------------------------------------
// - Get Sprite (Mutable)
//----------------------------------------------------------------------------
Sprite* Actor::getSprite()
{
    return sprite_;
}

//----------------------------------------------------------------------------
// - Get Sprite
//----------------------------------------------------------------------------
const Sprite* Actor::getSprite() const
{
    return sprite_;
}

//----------------------------------------------------------------------------
// - Set Facing
//----------------------------------------------------------------------------
// * direction : relative direction, counting 0 as east and rotating clockwise
//----------------------------------------------------------------------------
void Actor::face(int direction)
{
    baseSprite_->setDirection(direction);
}

//----------------------------------------------------------------------------
// - Face Toward Position
//----------------------------------------------------------------------------
// * target : x-y position to face toward by setting direction to the
//      closest angularly
//----------------------------------------------------------------------------
void Actor::face(const sf::Vector2f& target)
{
    double angle = atan2(target.y - position().y, target.x - position().x);

    if(angle < 0)
    {
        angle = 2 * M_PI + angle;
    }

    int direction = round(angle / 2 / M_PI * baseSprite_->getDirections());

    baseSprite_->setDirection(direction % baseSprite_->getDirections());
}

//----------------------------------------------------------------------------
// - Get Current Facing
//----------------------------------------------------------------------------
int Actor::facing() const
{
    return baseSprite_->getDirection();
}

//----------------------------------------------------------------------------
// - Get Environment
//----------------------------------------------------------------------------
const Map* Actor::getEnvironment() const
{
    return ground_;
}

//----------------------------------------------------------------------------
// - Get Portrait Sprite
//----------------------------------------------------------------------------
sf::Sprite* Actor::getPortrait() const
{
    return portrait_;
}

//----------------------------------------------------------------------------
// - Set Portrait Image
//----------------------------------------------------------------------------
// * portrait : bitmap to use for the portrait sprite
//----------------------------------------------------------------------------
void Actor::setPortrait(const sf::Texture& portrait)
{
    if(portrait_ == 0)
    {
        portrait_ = new sf::Sprite;
    }

    portrait_->setTexture(portrait);
    portrait_->setScale(64.f / portrait.getSize().x, 104.f / portrait.getSize().y);
}

//----------------------------------------------------------------------------
// - Get Actor's Name
//----------------------------------------------------------------------------
const std::string& Actor::getName() const
{
    return name_;
}

//----------------------------------------------------------------------------
// - Set Actor Name
//----------------------------------------------------------------------------
// * name : name of the actor
//----------------------------------------------------------------------------
void Actor::setName(const std::string& name)
{
    name_ = name;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*sprite_, states);
}

//----------------------------------------------------------------------------
// - Increment Frame
//----------------------------------------------------------------------------
// Update all motions, status checks and other frame-based operations
//----------------------------------------------------------------------------
void Actor::step()
{
    // Update path-based movement
    MobileObject::step();

    if(arrival_ == 0 && !path_.empty())
    {
        walkTo(path_.front());
        path_.pop_front();
    }    
}

//----------------------------------------------------------------------------
// - Position Occupiable
//----------------------------------------------------------------------------
// * position : position  actor is considering moving to
//----------------------------------------------------------------------------
bool Actor::occupiable(const sf::Vector2f& position) const
{
    if(ground_)
    {
        return !ground_->playerAt(position.x, position.y);
    }

    return true;
}

//----------------------------------------------------------------------------
// - Terrain Passable
//----------------------------------------------------------------------------
// * initial : position being traveled from
// * target : position being traveled to
// Returns True if the actor is able to move from one position to the other
//----------------------------------------------------------------------------
bool Actor::passable(const sf::Vector2f& initial, const sf::Vector2f& target) const
{
    if(ground_)
    {
        return ground_->valid(target.x, target.y);
    }

    return true;
}