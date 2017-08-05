#include "SpriteDirected.h"

//----------------------------------------------------------------------------
// - Directed Sprite Constructor
//----------------------------------------------------------------------------
// * texture : bitmap sprite sheet this sprite derives from
// * width : width in pixels of a single frame in the sheet
// * height : height in pixels of a single frame in the sheet
// * directions : number of orthagonal directions for this sprite sheet
//----------------------------------------------------------------------------
SpriteDirected::SpriteDirected(const sf::Texture & texture, int width, int height, int directions) :
	SpriteIndexed(texture)
{
	setDirections(directions);
	setWidth(width);
	setHeight(height);

    if(height_ != height)
    {
	    setHeight(heightLimit());	
    }
}

//----------------------------------------------------------------------------
// - Directed Sprite Destructor
//----------------------------------------------------------------------------
SpriteDirected::~SpriteDirected()
{
}

//----------------------------------------------------------------------------
// - Get Direction
//----------------------------------------------------------------------------
int SpriteDirected::getDirection() const
{
	return direction_;
}

//----------------------------------------------------------------------------
// - Set Direction
//----------------------------------------------------------------------------
// * direction : direction to have this sprite face, translating to the row
//       in the sheet [0, directions) from which to derive the indexed image 
//----------------------------------------------------------------------------
void SpriteDirected::setDirection(int direction)
{
	if (direction >= 0 && direction < directions_)
    {
        direction_ = direction;
        updateFrame();
    }
}

//----------------------------------------------------------------------------
// - Get Directions
//----------------------------------------------------------------------------
int SpriteDirected::getDirections() const
{
	return directions_;
}

//----------------------------------------------------------------------------
// - Set Number of Directions
//----------------------------------------------------------------------------
// * directions : orthagonal directions to splite the sheet into as rows
//----------------------------------------------------------------------------
void SpriteDirected::setDirections(int directions)
{
	if(directions > 0 && directions <= int(getTexture()->getSize().x * getTexture()->getSize().y))
    {
        directions_ = directions;
        direction_ = 0;

        updateFrame();
    }
}

//----------------------------------------------------------------------------
// - Get Index Limit (Override)
//----------------------------------------------------------------------------
// Override which takes directions into account, returning the index limit for
// a single row rather the entire texture
//----------------------------------------------------------------------------
int SpriteDirected::getIndexLimit() const
{
	return SpriteIndexed::getIndexLimit() / directions_;
}

//----------------------------------------------------------------------------
// - Get Height Limit (Override)
//----------------------------------------------------------------------------
// Override which takes directions into account, returning the height limit for
// a single row rather the entire texture
//----------------------------------------------------------------------------
int SpriteDirected::heightLimit() const
{
	return SpriteIndexed::heightLimit() / directions_;
}

//----------------------------------------------------------------------------
// - Update Texture Sub-Rectangle
//----------------------------------------------------------------------------
// Override which now includes splitting the sequences of images further into
// major rows as directions
//----------------------------------------------------------------------------
void SpriteDirected::updateFrame()
{
	int index = getIndex() + direction_ * getIndexLimit();
	int xTiles = getTexture()->getSize().x / getWidth();

	sf::Sprite::setTextureRect(sf::IntRect(
		getWidth() * (index % xTiles),
		getHeight() * (index / xTiles),
		getWidth(),
		getHeight())
    );
}


