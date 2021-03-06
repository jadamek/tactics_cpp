#include "Map.h"
#include <algorithm>
#include <math.h>

//----------------------------------------------------------------------------
// - Tilemap Constructor
//----------------------------------------------------------------------------
// * width : width of the map (x-axis) in tiles
// * length : length of the map (y-axis) in tiles
//----------------------------------------------------------------------------
Map::Map(int width, int length) :
    width_(std::max(1, width)),
    length_(std::max(1, length))
{
    tiles_.resize(width_, std::vector<std::vector<Tile*>>(length_));

    // 2-D player array
    actors_ = new Actor**[width_];
    for(int x = 0; x < width_; x++)
    {
        actors_[x] = new Actor*[length_];

        for(int y = 0; y < length_; y++)
        {
            actors_[x][y] = 0;
        }
    }
}

//----------------------------------------------------------------------------
// - Tilemap Destructor
//----------------------------------------------------------------------------
Map::~Map()
{
    // Delete all tiles
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
    // Delete 3-D actor array (not the actors themselves)
    for(int i = 0; i < width(); i++){
        delete [] actors_[i];
    }
    
    delete [] actors_;
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
// Valid Tile?
//----------------------------------------------------------------------------
// * x : x-coordinate of position by which to query for valid tile presence
// * y : y-coordinate of position by which to query for valid tile presence
// Returns whether the (x, y) position contains a valid tile
//----------------------------------------------------------------------------
bool Map::valid(float x, float y) const
{
    int x_i = (int)round(x);
    int y_i = (int)round(y);

    if((x < 0 || x >= width_) || (y < 0 || y >= length_))
    {
        return false;
    }
    
    return !tiles_[x][y].empty();
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
// - Get Isometric Depth Buffer
//----------------------------------------------------------------------------
IsometricBuffer& Map::getDepthBuffer()
{
    return images_;
}

//----------------------------------------------------------------------------
// - Get Player At Position
//----------------------------------------------------------------------------
Actor* Map::playerAt(int x, int y) const
{
    if((x >= 0 && x < width_) || (y >= 0 || y < length_))    
    {
        return actors_[x][y];
    }
    else{
        return 0;
    }
}

//----------------------------------------------------------------------------
// - Enter Player
//----------------------------------------------------------------------------
// * actor : player entering the map square
// * x : x-coordinate of the map square
// * y : y-coordinate of the map square
//----------------------------------------------------------------------------
void Map::enter(Actor* actor, int x, int y)
{
    if((x >= 0 && x < width_) || (y >= 0 || y < length_))
    {
        actors_[x][y] = actor;        
    }
}

//----------------------------------------------------------------------------
// - Exit Player
//----------------------------------------------------------------------------
// * x : x-coordinate of the map square the player is exiting
// * y : y-coordinate of the map square the player is exiting
//----------------------------------------------------------------------------
void Map::exit(int x, int y)
{
    if((x >= 0 && x < width_) || (y >= 0 || y < length_))
    {
        actors_[x][y] = 0;        
    }
}

//----------------------------------------------------------------------------
// - Draw (Override)
//----------------------------------------------------------------------------
void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(images_, states);
}
