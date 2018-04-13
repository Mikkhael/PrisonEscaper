#ifndef WALLACTOR_HPP_INCLUDED
#define WALLACTOR_HPP_INCLUDED

#include "Object.hpp"

struct WallType
{
	std::string textureName;
	sf::IntRect defaultRect;
	
	WallType(const std::string& textureName_, const sf::IntRect& defaultRect_)
		: textureName(textureName_), defaultRect(defaultRect_)
	{}
	
};

namespace WallTypes
{
	const WallType Bricks	("assets/walls/brick.bmp", 		sf::IntRect(0, 0, 8, 8));
	const WallType Rocks	("assets/walls/rocks.bmp", 		sf::IntRect(0, 0, 8, 8));
}


class WallActor : public SpriteActor
{
public:
	
	void setSize(const Vector2i& newSize)
	{
		sprite.setTextureRect(sf::IntRect(sprite.getTextureRect().left, sprite.getTextureRect().top, newSize.x, newSize.y));
	}
	Vector2i getSize()
	{
		return Rect<int>(sprite.getTextureRect()).size;
	}
	
	void setOffset(const Vector2i& newOffset)
	{
		sprite.setTextureRect(sf::IntRect(newOffset.x, newOffset.y, sprite.getTextureRect().width, sprite.getTextureRect().height));
	}
	Vector2i getOffset()
	{
		return Rect<int>(sprite.getTextureRect()).position;
	}
	
	
	WallActor(const sf::Texture& texture, const sf::IntRect& rect)
		: SpriteActor(texture, rect)
	{}
	
	WallActor(const WallType& wallType)
		: SpriteActor(textureManager.get(wallType.textureName, true), wallType.defaultRect)
	{}
};

#endif // WALLACTOR_HPP_INCLUDED
