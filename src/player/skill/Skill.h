#ifndef TACTICS_PLAYER_SKILL_H
#define TACTICS_PLAYER_SKILL_H

#include<SFML/Graphics.hpp>
#include<vector>

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
    Skill();
    virtual ~Skill();

    virtual void use(Actor& caster, const std::vector<Actor*>& targets) = 0;
    virtual std::vector<sf::Vector2i> range(Actor& caster) const = 0;
    virtual std::vector<sf::Vector2i> area(const sf::Vector2i& source, Map& map) const = 0;

    bool    casting() const;

private:
    void    setCastingStatus(bool casting);
    bool    casting_;
};

#endif