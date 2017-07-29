#include "Map.h"
#include <algorithm>
#include <math.h>

//----------------------------------------------------------------------------
// - Tilemap Constructor
//----------------------------------------------------------------------------
// * width : width of the map (x-axis) in tiles
// * length : length of the map (y-axis) in tiles
//----------------------------------------------------------------------------
Map::Map(int width, int length, const sf::Vector3f& scale) :
    width_(std::max(1, width)),
    length_(std::max(1, length)),
    images_(scale)
{
    tiles_.resize(width_, std::vector<std::vector<Tile*>>(length_));
}

//----------------------------------------------------------------------------
// - Tilemap Destructor
//----------------------------------------------------------------------------
Map::~Map()
{
    for(std::vector<std::vector<Tile*>> &column : tiles_)
    {
        for(std::vector<Tile*> &row : column)
        {
            for(Tile* tile : row)
            {
                delete tile;
            }
        }
    }
}

//----------------------------------------------------------------------------
// - Get Tile At
//----------------------------------------------------------------------------
// * x : x-coordinate of the tile to grab
// * y : y-coordinate of the tile to grab
// * z : z-coordinate of the tile to grab. If ommitted, grabs top-most
//----------------------------------------------------------------------------
const Tile* Map::at(int x, int y, float z) const
{
    if((x < 0 || x >= width_) || (y < 0 || y >= length_))
    {
        return 0;
    }
    else if(tiles_[x][y].size() > 0)
    {
        return 0;
    }
    else
    {
        int i = 0;
        // try to get the tile whose z -> height region contains
        // z. If z is above the top-most tile, return the top-most.
        while(z > tiles_[x][y][i]->position().z + tiles_[x][y][i]->getHeight()
                && i < tiles_[x][y].size() - 1)
        {            
            i += 1;
        }

        return tiles_[x][y][i];
    }
}

//----------------------------------------------------------------------------
// - Get Tile At (Mutable variant)
//----------------------------------------------------------------------------    
Tile* Map::at(int x, int y, float z)
{
    if((x < 0 || x >= width_) || (y < 0 || y >= length_))
    {
        return 0;
    }
    else if(tiles_[x][y].size() > 0)
    {
        return 0;
    }
    else
    {
        int i = 0;
        // try to get the tile whose z -> height region contains
        // z. If z is above the top-most tile, return the top-most.
        while(z > tiles_[x][y][i]->position().z + tiles_[x][y][i]->getHeight()
                && i < tiles_[x][y].size() - 1)
        {            
            i += 1;
        }

        return tiles_[x][y][i];
    }
}
#include <iostream>
//----------------------------------------------------------------------------
// - Place Tile
//----------------------------------------------------------------------------
// * tile : a tile to be placed on the top-most layer
// * x : x-coordinate for the position to place the new tile at
// * y : y-coordinate for the position to place the new tile at
//----------------------------------------------------------------------------
bool Map::place(Tile* tile, int x, int y)
{
    if((x < 0 || x >= width_) || (y < 0 || y >= length_))
    {
        return false;
    }

    float z = 0;

    // Place on the exact top of the highest tile at (x, y)
    if(tiles_[x][y].size() > 0)
    {
        std::cout << "   already a tile here ... " << std::endl;
        z = tiles_[x][y].back()->position().z + tiles_[x][y].back()->getHeight();
        tile->setOccupant(tiles_[x][y].back()->getOccupant());
        tiles_[x][y].back()->setOccupant(tile);

        if(tile->getOccupant() != 0)
        {
            tile->getOccupant()->rise(tile->getHeight());
        }
    }

    tile->setPosition(sf::Vector3f(x, y, z));

    tiles_[x][y].push_back(tile);
    images_.add(tile);

    return true;
}

//----------------------------------------------------------------------------
// - Insert Tile
//----------------------------------------------------------------------------
// * tile : a tile to be inserted at the specified position and layer
// * x : x-coordinate to insert the new tile at
// * y : y-coordinate to insert the new tile at
// * layer : layer (index) to insert the tile at
//----------------------------------------------------------------------------
bool Map::insert(Tile* tile, int x, int y, int layer)
{
    if((x < 0 || x >= width_) || (y < 0 || y >= length_))
    {
        return false;
    }
    else if(layer < 0 || layer >= tiles_[x][y].size())
    {
        return false;
    }

    float z = 0;    

    return true;
}

//----------------------------------------------------------------------------
// - Override Tile
//----------------------------------------------------------------------------
// * tile : the tile that will replace another
// * x : x-coordinate of the tile to be replaced
// * y : y-coordinate of the tile to be replaced
// * layer : layer (index) of the tile to be replaced
//----------------------------------------------------------------------------
bool Map::replace(Tile* tile, int x, int y, int layer)
{
    return true;
}

//----------------------------------------------------------------------------
// - Remove Tile
//----------------------------------------------------------------------------
// * x : x-coordinate of the tile to be removed
// * y : y-coordinate of the tile to be removed
// * layer : layer (index) of the tile to be removed
//----------------------------------------------------------------------------
bool Map::remove(int x, int y, int layer)
{
    return true;
}

//----------------------------------------------------------------------------
// Get Height
//----------------------------------------------------------------------------
// * x : x-coordinate of position by which to query the map height.
// * y : y-coordinate of position by which to query the map height.
// Returns the height of an (x, y) position on the map, or -1 if the space
// is out of bounds or unfilled.
//----------------------------------------------------------------------------
float Map::height(float x, float y) const
{
    int x_i = (int)round(x);
    int y_i = (int)round(y);
    const Tile* tile = at(x_i, y_i);
    if(tile)
    {
        return tile->position().z + tile->getHeight(sf::Vector2f(x - x_i, y - y_i));
    }
    else
    {
        return -1;
    }
}

//----------------------------------------------------------------------------
// Add Object
//----------------------------------------------------------------------------
// * obj : non-tile object to add to this map's depth buffer
//----------------------------------------------------------------------------
void Map::addObject(const IsometricObject* obj)
{
    images_.add(obj);
}

//----------------------------------------------------------------------------
// - Get Map Width
//----------------------------------------------------------------------------
int Map::getWidth() const
{
    return width_;
}

//----------------------------------------------------------------------------
// - Get Map Length
//----------------------------------------------------------------------------
int Map::getLength() const
{
    return length_;
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(images_, states);
}
