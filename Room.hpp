#ifndef ROOM_HPP_INCLUDED
#define ROOM_HPP_INCLUDED

#include "Colisions.hpp"
#include "PLatform.hpp"
#include <vector>
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


class Room : public SpriteActor, public ActorCollection<Room>
{
	bool 		collidableBounds;
		
public:
	
	static bool autoOffsetWallTexture;
	
	Rect<double> getRect() const
	{
		return Rect<double>(getPosition(), getSize());
	}
	
	static bool isPointInside(const Vector2d& point)
	{
	    bool inside = false;
	    iterate([&point, &inside](const Room& room)
        {
            inside = CollisionFast::test(point, room.getRect());
            return inside;
        });
        return inside;
	}
	
	Room(const Rect<double>& rect_, const WallType& wallType)
		: SpriteActor(textureManager.get(wallType.textureName, true), wallType.defaultRect), collidableBounds(false)
	{
		
		setPosition(rect_.position);
		setSize(rect_.size);
		if(autoOffsetWallTexture){
            setOffset(rect_.position);
		}
	}
	Room(const Rect<double>& rect_, const WallType& wallType, std::vector<Platform>& platformsCollection)
		: SpriteActor(textureManager.get(wallType.textureName, true), wallType.defaultRect), collidableBounds(false)
	{
		setPosition(rect_.position);
		setSize(rect_.size);
		
		if(autoOffsetWallTexture){
            setOffset(rect_.position);
		}
		
		Rect<double> tempRect = getRect();
        platformsCollection.emplace_back(tempRect.getUpperLeft(),  tempRect.size.x, false);
        platformsCollection.emplace_back(tempRect.getBottomLeft(), tempRect.size.x, false);
        platformsCollection.emplace_back(tempRect.getUpperLeft(),  tempRect.size.y, true);
        platformsCollection.emplace_back(tempRect.getUpperRight(), tempRect.size.y, true);
		
	}
	
	
	virtual ~Room(){}
	
};

bool Room::autoOffsetWallTexture = true;

#endif // ROOM_HPP_INCLUDED
