#ifndef TACTICS_SPRITE_INDEXED_H
#define TACTICS_SPRITE_INDEXED_H

#include <SFML/Graphics.hpp>
#include "Sprite.h"

//================================================================================
// ** Sprite Indexed
//================================================================================
// A sprite which allows for indexed sub-frames
//================================================================================
class SpriteIndexed : public Sprite
{
// Methods
public:
	SpriteIndexed(const sf::Texture& texture, int width = 0, int height = 0);
	virtual ~SpriteIndexed();

	int				        getIndex() const;
	int				        getWidth() const;
	int				        getHeight() const;
	void			        setIndex(int index);
	void			        setWidth(int width);
	void			        setHeight(int height);
    const sf::Texture*      getTexture() const;
	void			        setTexture(const sf::Texture& texture);
    virtual sf::FloatRect   getGlobalBounds() const;
	virtual int		        getIndexLimit() const;

protected:
    void            draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual int		widthLimit() const;
	virtual int		heightLimit() const;
	virtual void	updateFrame();

// Members
	int				index_;
	int				width_;
	int				height_;
    sf::Sprite      sprite_;
};

#endif