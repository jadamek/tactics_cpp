#ifndef TACTICS_TILE_MAP_H
#define TACTICS_TILE_MAP_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <float.h>
#include "Tile.h"
#include "IsometricBuffer.h"

class Actor;

//================================================================================
// ** Map
//================================================================================
// Represents an isometric tilemap, which consists of a 2-D jagged array of 3-D
// isometric map objects represented as blocks.
//================================================================================
class Map : public sf::Drawable
{
// Methods
public:
    Map(int width = 10, int length = 10);
    virtual ~Map();

    const Tile*         at(int x, int y, float z = FLT_MAX) const;
    Tile*               at(int x, int y, float z = FLT_MAX);
    bool                place(Tile* tile, int x, int y);
    bool                insert(Tile* tile, int x, int y, int layer);
    bool                replace(Tile* tile, int x, int y, int layer);
    bool                remove(int x, int y, int layer);
    bool                valid(float x, float y) const;
    float               height(float x, float y) const;
    void                addObject(const IsometricObject* obj);
    int                 width() const;
    int                 length() const;
    IsometricBuffer&    getDepthBuffer();
    Actor*              playerAt(int x, int y) const;
    void                enter(Actor* actor, int x, int y);
    void                exit(int x, int y);
    
protected:
    virtual void        draw(sf::RenderTarget& target, sf::RenderStates states) const;
    Tile*               getTileAt(int x, int y, float z = FLT_MAX) const;

// Members
    int                 width_;
    int                 length_;
    IsometricBuffer     images_;
    std::vector<std::vector<std::vector<Tile*>>> tiles_;
    Actor***            players_;
};

#endif