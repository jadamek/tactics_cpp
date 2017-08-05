#ifndef TACTICS_SPRITE_DIRECTIONAL_H
#define TACTICS_SPRITE_DIRECTIONAL_H

#include "SpriteIndexed.h"

//================================================================================
// ** Directed Sprite
//================================================================================
// Extension which splits the sheet into orthagonal directions as rows
//================================================================================
class SpriteDirected : public SpriteIndexed
{
// Methods
public:
	SpriteDirected(const sf::Texture& texture, int width = 0, int height = 0, int directions = 4);
	virtual ~SpriteDirected();

	int 			getDirection() const;
	void 			setDirection(int direction);
	int 			getDirections() const;
	void 			setDirections(int directions);
	virtual int 	getIndexLimit() const;

protected:
	virtual int 	heightLimit() const;
	virtual void 	updateFrame();

// Members
	int 			directions_;
	int 			direction_;
};

#endif