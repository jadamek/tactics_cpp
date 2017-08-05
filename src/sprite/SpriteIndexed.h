#ifndef TACTICS_SPRITE_INDEXED_H
#define TACTICS_SPRITE_INDEXED_H

#include <SFML/Graphics.hpp>

//================================================================================
// ** Sprite Indexed
//================================================================================
// An extention of an SFML sprite which allows for indexed sub-frames
//================================================================================
class SpriteIndexed : public sf::Sprite
{
// Methods
	SpriteIndexed(const sf::Texture& texture, int width = 0, int height = 0);
	virtual ~SpriteIndexed();

	int				getIndex() const;
	int				getWidth() const;
	int				getHeight() const;
	void			setIndex(int index);
	void			setWidth(int width);
	void			setHeight(int height);
	void			setTexture(const sf::Texture& texture);

protected:
	virtual int		indexLimit() const;
	virtual int		widthLimit() const;
	virtual int		heightLimit() const;
	virtual void	updateFrame();

// Members
private:
	int				index_;
	int				width_;
	int				height_;

	// Override - Undefined for Indexed sprites.
	void			setTextureRect(const sf::IntRect& rectangle) {}
};

#endif