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
Tile* Map::getTileAt(int x, int y, float z) const
{
    if((x < 0 || x >= width_) || (y < 0 || y >= length_))
    {
        return 0;
    }
    else if(tiles_[x][y].empty())
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
// - Get Tile At (Const-Interface)
//----------------------------------------------------------------------------
const Tile* Map::at(int x, int y, float z) const
{
    return getTileAt(x, y, z);
}

//----------------------------------------------------------------------------
// - Get Tile At (Mutable Interface)
//----------------------------------------------------------------------------    
Tile* Map::at(int x, int y, float z)
{
    return getTileAt(x, y, z);    
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
    else if(layer < 0 || layer > tiles_[x][y].size())
    {
        return false;
    }

    float z = 0;    

    if(layer > 0)
    {
        z = tiles_[x][y][layer - 1]->position().z + tiles_[x][y][layer - 1]->getHeight();
        tiles_[x][y][layer-1]->setOccupant(tile);
    }

    if(layer < tiles_[x][y].size())
    {
        tile->setOccupant(tiles_[x][y][layer]);
        tile->getOccupant()->rise(tile->getHeight());
    }

    tile->setPosition(sf::Vector3f(x, y, z));
    tiles_[x][y].insert(tiles_[x][y].begin() + layer, tile);
    images_.add(tile);

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
    if((x < 0 || x >= width_) || (y < 0 || y >= length_))
    {
        return false;
    }
    else if(layer < 0 || layer >= tiles_[x][y].size())
    {
        return false;
    }

    if(layer > 0)
    {
        tiles_[x][y][layer - 1]->setOccupant(tile);
    }

    tile->setOccupant(tiles_[x][y][layer]->getOccupant());

    if(tile->getOccupant() != 0)
    {
        float difference = tile->getHeight() - tiles_[x][y][layer]->getHeight();

        if(difference > 0)
        {
            tile->getOccupant()->rise(difference);
        }
        else
        {
            tile->getOccupant()->lower(difference * -1);
        }
    }

    tile->setPosition(tiles_[x][y][layer]->position());

    images_.remove(tiles_[x][y][layer]);
    delete tiles_[x][y][layer];
    tiles_[x][y][layer] = tile;
    images_.add(tile);

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
    if((x < 0 || x >= width_) || (y < 0 || y >= length_))
    {
        return false;
    }
    else if(layer < 0 || layer >= tiles_[x][y].size())
    {
        return false;
    }
    std::cout << "removing tile at (" << x << ", " << y << ", " << layer << ")" << std::endl;
    if(layer > 0)
    {
        tiles_[x][y][layer - 1]->setOccupant(tiles_[x][y][layer]->getOccupant());
    }

    if(tiles_[x][y][layer]->getOccupant() != 0)
    {
        tiles_[x][y][layer]->getOccupant()->lower(tiles_[x][y][layer]->getHeight());
    }

    images_.remove(tiles_[x][y][layer]);
    delete tiles_[x][y][layer];
    tiles_[x][y].erase(tiles_[x][y].begin() + layer);

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
int Map::width() const
{
    return width_;
}

//----------------------------------------------------------------------------
// - Get Map Length
//----------------------------------------------------------------------------
int Map::length() const
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
