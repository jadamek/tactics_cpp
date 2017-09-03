#include "Actor.h"
#include "../player/skill/SkillAttack.h"
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
    name_("Combatant"),
    attrMove_(4)
{
    baseSprite_->setOrigin(10, 28);
    baseSprite_->setPosition(0, 0);
    sprite_ = new SpriteAnimated(baseSprite_);
    sprite_->play("default", true);

    skills_.push_back(new SkillAttack);
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
    for(Skill* skill : skills_)
    {
        delete skill;
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
void Actor::walkAlong(const std::deque<sf::Vector2f>& path)
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
// - Structure for BFS Search during reach computation
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
    std::vector<sf::Vector2f> area;
    sf::Vector2f origin = sf::Vector2f(position().x, position().y) - sf::Vector2f(attrMove_, attrMove_);
    int width = 2 * attrMove_ + 1;
    std::vector<bool> visited(width * width, false);
    std::queue<BFSToken> queue;

    queue.push(BFSToken{sf::Vector2f(position().x, position().y), attrMove_});
    visited[attrMove_ * (1 +  width)] = true;

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
            v_i = int(token.position.x - origin.x) + int(token.position.y - origin.y) * width;

            // Check Rightward Position
            adjacent.x = token.position.x + 1;
            adjacent.y = token.position.y;
            if((adjacent.x >= 0 && adjacent.y >= 0))
            {
                if(!visited[v_i + 1] && passable(token.position, adjacent)){
                    visited[v_i + 1] = true;
                    queue.push(BFSToken{adjacent, token.distance - 1});
                }
            }
            
            // Check Leftward Position
            adjacent.x = token.position.x - 1;
            adjacent.y = token.position.y;
            if((adjacent.x >= 0 && adjacent.y >= 0))
            {
                if(!visited[v_i - 1] && passable(token.position, adjacent)){
                    visited[v_i - 1] = true;
                    queue.push(BFSToken{adjacent, token.distance - 1});
                }
            }
    
            // Check Downward Position
            adjacent.x = token.position.x;
            adjacent.y = token.position.y + 1;
            {
                if(!visited[v_i + width] && passable(token.position, adjacent)){
                    visited[v_i + width] = true;
                    queue.push(BFSToken{adjacent, token.distance - 1});
                }
            }
    
            // Check Upward Position
            adjacent.x = token.position.x;
            adjacent.y = token.position.y - 1;
            if((adjacent.x >= 0 && adjacent.y >= 0))
            {
                if(!visited[v_i - width] && passable(token.position, adjacent)){
                    visited[v_i - width] = true;
                    queue.push(BFSToken{adjacent, token.distance - 1});
                }
            }
        }        
    }

    return area;
}

//----------------------------------------------------------------------------
// - Structure for BFS Search during shortest path computation
//----------------------------------------------------------------------------
struct BFSNode{
    bool visited;
    int parent;
    sf::Vector2f position;
};

//----------------------------------------------------------------------------
// - Compute Shorest Path
//----------------------------------------------------------------------------
// Returns a deque of the shortest path to a given destination given this
// unit's movement using breadth-first search guided by passability laws
//----------------------------------------------------------------------------
std::deque<sf::Vector2f> Actor::shortestPath(const sf::Vector2f& destination) const
{    
    std::deque<sf::Vector2f> path;
    sf::Vector2f source(position().x, position().y);
    int tracer = -1;
    
    // Make sure the destination is reachable, as well as not the current position
    int distance = abs(position().x - destination.x) + abs(position().y - destination.y);

    if(source != destination && distance <= attrMove_)
    {        
        std::vector<sf::Vector2f> area;
        sf::Vector2f origin = source - sf::Vector2f(attrMove_, attrMove_);
        int width = 2 * attrMove_ + 1;
        std::vector<BFSNode> visited(width * width, {false, -1, sf::Vector2f()});
        std::queue<BFSToken> queue;
    
        // Add the source position
        queue.push(BFSToken{source, attrMove_});
        visited[attrMove_ * (1 +  width)] = {true, -1, source};
    
        // Loop variables    
        BFSToken token;
        sf::Vector2f adjacent;
        int v_i;
    
        // BFS Search until the destination is found
        while(!queue.empty())
        {
            token = queue.front();
            queue.pop();

            v_i = int(token.position.x - origin.x) + int(token.position.y - origin.y) * width;
            
            // Once found, end BFS search
            if(token.position == destination)
            {
                tracer = v_i;
                break;
            }
    
            if(token.distance > 0)
            {    
                // Check Rightward Position
                adjacent.x = token.position.x + 1;
                adjacent.y = token.position.y;
                if((adjacent.x >= 0 && adjacent.y >= 0))
                {
                    if(!visited[v_i + 1].visited && passable(token.position, adjacent)){
                        visited[v_i + 1] = {true, v_i, adjacent};
                        queue.push(BFSToken{adjacent, token.distance - 1});
                    }
                }
                
                // Check Leftward Position
                adjacent.x = token.position.x - 1;
                adjacent.y = token.position.y;
                if((adjacent.x >= 0 && adjacent.y >= 0))
                {
                    if(!visited[v_i - 1].visited && passable(token.position, adjacent)){
                        visited[v_i - 1] = {true, v_i, adjacent};
                        queue.push(BFSToken{adjacent, token.distance - 1});
                    }
                }
        
                // Check Downward Position
                adjacent.x = token.position.x;
                adjacent.y = token.position.y + 1;
                {
                    if(!visited[v_i + width].visited && passable(token.position, adjacent)){
                        visited[v_i + width] = {true, v_i, adjacent};
                        queue.push(BFSToken{adjacent, token.distance - 1});
                    }
                }
        
                // Check Upward Position
                adjacent.x = token.position.x;
                adjacent.y = token.position.y - 1;
                if((adjacent.x >= 0 && adjacent.y >= 0))
                {
                    if(!visited[v_i - width].visited && passable(token.position, adjacent)){
                        visited[v_i - width] = {true, v_i, adjacent};
                        queue.push(BFSToken{adjacent, token.distance - 1});
                    }
                }
            }        
        }

        // If the destination was found, trace backward to discover the path from the source
        while(tracer != -1)
        {
            path.push_front(visited[tracer].position);
            tracer = visited[tracer].parent;
        }
    }

    return path;
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
// - Get Move Attribute
//----------------------------------------------------------------------------
int Actor::getMove() const
{
    return attrMove_;
}

//----------------------------------------------------------------------------
// - Set Move Attribute
//----------------------------------------------------------------------------
// mv : maximum number of spaces this actor can move in a single turn
//----------------------------------------------------------------------------
void Actor::setMove(int mv)
{
    if(mv >= 0)
    {
        attrMove_ = mv;
    }
}

//----------------------------------------------------------------------------
// - Get Learned Skills
//----------------------------------------------------------------------------
const std::vector<Skill*>& Actor::getSkills() const
{
    return skills_;
}

//----------------------------------------------------------------------------
// - Highlight Actor
//----------------------------------------------------------------------------
// Produces a visual effect on the player's sprite to indicate it is being
// targetted by a skill
//----------------------------------------------------------------------------
void Actor::focus()
{
    baseSprite_->setColor(sf::Color(255, 150, 150));
}

//----------------------------------------------------------------------------
// - Remove Actor Highlighting
//----------------------------------------------------------------------------
void Actor::unfocus()
{
    baseSprite_->setColor(sf::Color::White);
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