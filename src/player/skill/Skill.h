#ifndef TACTICS_PLAYER_SKILL_H
#define TACTICS_PLAYER_SKILL_H

#include<SFML/Graphics.hpp>
#include<vector>
#include<string>

class Actor;
class Map;

//================================================================================
// ** Skill
//================================================================================
// Abstract base for all usable skills, ranging from basic attacks to cast magic
//================================================================================
class Skill
{
// Methods
public:
    Skill(Actor& caster);
    virtual ~Skill();

    virtual void                        use(const std::vector<Actor*>& targets) = 0;
    virtual std::vector<sf::Vector2f>   range() const = 0;
    virtual std::vector<sf::Vector2f>   area(const sf::Vector3f& target) const = 0;
    virtual std::vector<Actor*>         affected(const sf::Vector3f& target) const;
    virtual bool                        effective(const sf::Vector3f& target) const;
    
    Actor*                              caster();
    bool                                casting() const;
    std::string                         name() const;    

protected:
    void        setCastingStatus(bool casting);

// Members
    Actor*      caster_;
    bool        casting_;
    std::string name_;
};

#endif