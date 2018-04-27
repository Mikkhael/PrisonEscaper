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
	
	Room(const Rect<double>& rect_, const WallType& wallType, short collidableBounds_ = true)
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
	
	static merge(Room& room1, Room& room2)
	{
	    int r1size = room1.platforms.size();
	    int r2size = room2.platforms.size();
	    for(int i=0; i<r1size; i++)
        {
            for(int j=0; j<r2size; j++)
            {
                SimpleSegment<double>& s1 = (room1.platforms[i].segment.getStartValue() <= room2.platforms[j].segment.getStartValue()) ? room1.platforms[i].segment : room2.platforms[j].segment;
                SimpleSegment<double>& s2 = (room1.platforms[i].segment.getStartValue() <= room2.platforms[j].segment.getStartValue()) ? room2.platforms[j].segment : room1.platforms[i].segment;
                
                if(s1.isVertical != s2.isVertical || s1.length < 0.001 || s2.length < 0.001)
                {
                    continue;
                }
                
                Collision::Result overlap = Collision::test(s1, s2, 0.001);
                if(overlap)
                {
                    //std::cout << s1.position.x << "  " <<s1.position.y << " : " <<s1.length << " --- " << s2.position.x << " "<<s2.position.y << " : " <<s2.length <<std::endl;
                    std::cout << s1.getStartValue() << "  " <<s1.getEndValue() << " : " <<s1.getOffset() << " --- " << s2.getStartValue() << " "<<s2.getEndValue() << " : " <<s2.getOffset() <<std::endl;
                    if(s1.getEndValue() <= s2.getEndValue())
                    {
                        double tempEnd = s1.getEndValue();
                        s1.setEndValue(s2.getStartValue());
                        s2.setStartValue(tempEnd);
                    }
                    else
                    {
                        double tempEnd = s1.getEndValue();
                        s1.setEndValue(s2.getStartValue());
                        s2.setStartValue(s2.getEndValue());
                        s2.setEndValue(tempEnd);
                    }             
                    //std::cout << s1.position.x << "  " <<s1.position.y << " : " <<s1.length << " --- " << s2.position.x << " "<<s2.position.y << " : " <<s2.length <<std::endl;
                    std::cout << s1.getStartValue() << "  " <<s1.getEndValue() << " : " <<s1.getOffset() << " --- " << s2.getStartValue() << " "<<s2.getEndValue() << " : " <<s2.getOffset() <<std::endl;
                    std::cout << "___________________________" <<std::endl;   
                }
                
            }
        }
	}
	
	static mergeAll(std::vector<Room>& rooms)
	{
	    for(int i=0; i<rooms.size(); i++)
        {
            for(int j=i+1; j<rooms.size(); j++)
            {
                merge(rooms[i], rooms[j]);
                
            }
        }
	    
	}
};


#endif // ROOM_HPP_INCLUDED
