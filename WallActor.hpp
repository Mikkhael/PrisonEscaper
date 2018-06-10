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
	
	Rect<double> getRect() const
	{
		return Rect<double>(getPosition(), getSize());
	}
	
	
	WallActor(const sf::Texture& texture, const sf::IntRect& rect)
		: SpriteActor(texture, rect)
	{}
	
	WallActor(const WallType& wallType)
		: SpriteActor(textureManager.get(wallType.textureName, true), wallType.defaultRect)
	{}
	
	virtual ~WallActor(){}
};

#endif // WALLACTOR_HPP_INCLUDED
