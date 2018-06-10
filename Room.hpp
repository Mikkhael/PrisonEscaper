#ifndef ROOM_HPP_INCLUDED
#define ROOM_HPP_INCLUDED

#include "WallActor.hpp"
#include "Colisions.hpp"
#include "PLatform.hpp"
#include <vector>



class Room
{
	WallActor 	wall;
	bool 		collidableBounds;
	
public:
	
	static bool autoOffsetWallTexture;
	
	void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
	{
		wall.draw(target, states);
	}
	
	Room(const Rect<double>& rect_, const WallType& wallType)
		: wall(wallType), collidableBounds(false)
	{
		
		std::cout << "New Room" << std::endl;
		wall.setPosition(rect_.position);
		wall.setSize(rect_.size);
		if(autoOffsetWallTexture){
            wall.setOffset(rect_.position);
		std::cout << "End New Room" << std::endl;
		}
	}
	Room(const Rect<double>& rect_, const WallType& wallType, std::vector<Platform>& platformsCollection)
		: wall(wallType), collidableBounds(false)
	{
		wall.setPosition(rect_.position);
		wall.setSize(rect_.size);
		
		if(autoOffsetWallTexture){
            wall.setOffset(rect_.position);
		}
		
		Rect<double> tempRect = wall.getRect();
        platformsCollection.emplace_back(tempRect.getUpperLeft(),  tempRect.size.x, false);
        platformsCollection.emplace_back(tempRect.getBottomLeft(), tempRect.size.x, false);
        platformsCollection.emplace_back(tempRect.getUpperLeft(),  tempRect.size.y, true);
        platformsCollection.emplace_back(tempRect.getUpperRight(), tempRect.size.y, true);
		
	}
	
	virtual ~Room(){}
	
};

bool Room::autoOffsetWallTexture = true;
std::vector<Room> rooms;
std::vector<Platform> platforms;

#endif // ROOM_HPP_INCLUDED
