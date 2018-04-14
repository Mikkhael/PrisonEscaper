#ifndef ROOM_HPP_INCLUDED
#define ROOM_HPP_INCLUDED

#include "WallActor.hpp"
#include <vector>

#define DRAW_PLATFORMS

class Platform
{
public:
	
	SimpleSegment<double> 	segment;
	
	Platform(const Vector2d& position, double length, bool isVertical)
		: segment(position, length, isVertical)
	{
		
	}
	
	#ifdef DRAW_PLATFORMS
	
	void draw(sf::RenderTarget& rt) const
	{
		sf::RectangleShape shape(segment.getVector());
		shape.setPosition(segment.position);
		shape.setOutlineThickness(2);
		shape.setOutlineColor(sf::Color::Red);
		
		rt.draw(shape);
	}
	
	#endif // DRAW_PLATFORMS
};


class Room
{
	WallActor 	wall;
	bool 		collidableBounds;
	
public:
	
	std::vector<Platform> platforms;
	
	void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
	{
		wall.draw(target, states);
		
		#ifdef DRAW_PLATFORMS
		
		for(auto& platform : platforms)
		{
			platform.draw(target);
		}
		
		#endif // DRAW_PLATFORMS
	}
	
	Room(const Rect<double>& rect_, const WallType& wallType, bool collidableBounds_ = true)
		: wall(wallType), collidableBounds(collidableBounds_)
	{
		wall.setPosition(rect_.position);
		wall.setSize(rect_.size);
		
		if(collidableBounds)
		{
			Rect<double> tempRect = wall.getRect();
			platforms.push_back(Platform(tempRect.getUpperLeft(),  tempRect.size.x, false));
			platforms.push_back(Platform(tempRect.getUpperLeft(),  tempRect.size.y, true));
			platforms.push_back(Platform(tempRect.getBottomLeft(), tempRect.size.x, false));
			platforms.push_back(Platform(tempRect.getUpperRight(), tempRect.size.y, true));
		}
		
	}
};


#endif // ROOM_HPP_INCLUDED
