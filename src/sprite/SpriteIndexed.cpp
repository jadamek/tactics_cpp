#include "SpriteIndexed.h"

//----------------------------------------------------------------------------
// - Indexed Sprite Constructor
//----------------------------------------------------------------------------
// * texture : bitmap sprite sheet this sprite derives from
// * width : width in pixels of a single frame in the sheet
// * height : height in pixels of a single frame in the sheet
//----------------------------------------------------------------------------
SpriteIndexed::SpriteIndexed(const sf::Texture &texture, int width, int height) :
	sf::Sprite(texture),
	index_(0),
	width_((0 < width  && width < widthLimit())  ? width  : texture.getSize().x),
	height_ = ((0 < height && height < heightLimit()) ? height : texture.getSize().y)
{
	sf::Sprite::setTextureRect(sf::IntRect(0, 0, width_, height_));
}

//----------------------------------------------------------------------------
// - Indexed Sprite Destructor
//----------------------------------------------------------------------------
SpriteIndexed::~SpriteIndexed()
{
}

//----------------------------------------------------------------------------
// - Get Index
//----------------------------------------------------------------------------
int SpriteIndexed::getIndex() const
{
	return index_;
}

//----------------------------------------------------------------------------
// - Set Index
//----------------------------------------------------------------------------
// * index : frame within the sheet to display, starting from the top left of
//      the image, counting left -> right, top -> bottom splitting the image
//      an array of frames defined by the sprite's width and height
//----------------------------------------------------------------------------
void SpriteIndexed::setIndex(int index)
{
	if (index >= 0 && index < indexLimit())
    {
        index_ = index;
        updateFrame();
    }
}

//----------------------------------------------------------------------------
// - Get Width
//----------------------------------------------------------------------------
int SpriteIndexed::getWidth() const
{
	return width_;
}


//----------------------------------------------------------------------------
// - Set Width
//----------------------------------------------------------------------------
// * width : width in pixels of a single frame in the sheet
//----------------------------------------------------------------------------
void SpriteIndexed::setWidth(int width)
{
	if (width > 0 && width <= widthLimit())
    {
        width_ = width;
        index_ = 0;
        updateFrame();
    }
}

//----------------------------------------------------------------------------
// - Get Height
//----------------------------------------------------------------------------
int SpriteIndexed::getHeight() const
{
	return height_;
}

//----------------------------------------------------------------------------
// - Set Height
//----------------------------------------------------------------------------
// * height : height in pixels of a single frame in the sheet
//----------------------------------------------------------------------------
void SpriteIndexed::setHeight(int height)
{
	if (height > 0 && height <= heightLimit())
    {
        height_ = height;
        index_ = 0;
        updateFrame();
    }
}

//----------------------------------------------------------------------------
// - Set Texture (Override)
//----------------------------------------------------------------------------
// * texture : bitmap sprite sheet this sprite derives from
// Overrides set texture to handle any indexing discrepancies
//----------------------------------------------------------------------------
void SpriteIndexed::setTexture(const sf::Texture & texture)
{
	sf::Sprite::setTexture(texture);

	if (width_ < 1 || width_ > widthLimit())
    {
		width_ = widthLimit();
    }

	if (height_ < 1 || height_ > heightLimit())
    {
		height_ = heightLimit();
    }

	if (index_ > indexLimit())
    {
		index_ = indexLimit();
    }
}

//----------------------------------------------------------------------------
// - Get Index Limit
//----------------------------------------------------------------------------
// Computes the maximum possible index given the width and height of a frame
// and the size of the source texture
//----------------------------------------------------------------------------
int SpriteIndexed::indexLimit() const
{
	if (!getTexture())
		return 0;

	return (getTexture()->getSize().x / width_) * (getTexture()->getSize().y / height_);
}

//----------------------------------------------------------------------------
// - Get Width Limit
//----------------------------------------------------------------------------
// Computes the maximum possible width of a frame from the source texture
//----------------------------------------------------------------------------
int SpriteIndexed::widthLimit() const
{
	if (!getTexture())
		return 0;

	return getTexture()->getSize().x;
}

//----------------------------------------------------------------------------
// - Get Height
//----------------------------------------------------------------------------
// Computes the maximum possible height of a frame from the source texture
//----------------------------------------------------------------------------
int SpriteIndexed::heightLimit() const
{
	if (!getTexture())
		return 0;

	return getTexture()->getSize().y;
}

//----------------------------------------------------------------------------
// - Update Texture Sub-Rectangle
//----------------------------------------------------------------------------
// Sets the sprite's texture sub-rectangle to the frame located at the given
// index, counting left -> right, then top -> bottom (row)
//----------------------------------------------------------------------------
void SpriteIndexed::updateFrame()
{
	int xTiles = getTexture()->getSize().x / width_;
	sf::Sprite::setTextureRect(sf::IntRect(width_ * (index_ % xTiles), height_ * (index_ / xTiles), width_, height_));
}
