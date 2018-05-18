#ifndef ROOM_HPP_INCLUDED
#define ROOM_HPP_INCLUDED

#include "WallActor.hpp"
#include "Colisions.hpp"
#include <vector>
#include <algorithm>

#define DRAW_PLATFORMS

class Platform : public FixedSimpleSegmentCollider
{
    
public:
	
	static void merge(Platform& platform1, Platform& platform2)
	{
	    
            SimpleSegment<double>& s1 = (platform1.collider.getStartValue() <= platform2.collider.getStartValue()) ? platform1.collider : platform2.collider;
            SimpleSegment<double>& s2 = (platform1.collider.getStartValue() <= platform2.collider.getStartValue()) ? platform2.collider : platform1.collider;
            
            if(s1.isVertical != s2.isVertical || s1.length < 0.001 || s2.length < 0.001)
            {
                return;
            }
            
            Collision::Result overlap = Collision::test(s1, s2, 0.001);
            if(overlap)
            {
                //std::cout << s1.position.x << "  " <<s1.position.y << " : " <<s1.length << " --- " << s2.position.x << " "<<s2.position.y << " : " <<s2.length <<std::endl;
                //std::cout << s1.getStartValue() << "  " <<s1.getEndValue() << " : " <<s1.getOffset() << " --- " << s2.getStartValue() << " "<<s2.getEndValue() << " : " <<s2.getOffset() <<std::endl;
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
                //std::cout << s1.getStartValue() << "  " <<s1.getEndValue() << " : " <<s1.getOffset() << " --- " << s2.getStartValue() << " "<<s2.getEndValue() << " : " <<s2.getOffset() <<std::endl;
                //std::cout << "___________________________" <<std::endl;   
            }
                
	}
	
	static void mergeAll(std::vector<Platform>& platforms)
	{
	    for(auto i=0u; i<platforms.size(); i++)
        {
            for(auto j=i+1u; j<platforms.size(); j++)
            {
                merge(platforms[i], platforms[j]);
            }
        }
        
        platforms.erase(std::remove_if(platforms.begin(), platforms.end(), [](const Platform& platform){return platform.collider.length <= 0.001;}), platforms.end());
	}
	
	
	Platform(const Vector2d& position, double length, bool isVertical)
		: FixedSimpleSegmentCollider(SimpleSegment<double>(position, length, isVertical))
	{
		
	}
	
	#ifdef DRAW_PLATFORMS
	
    bool isYellow = false;
	void draw(sf::RenderTarget& rt) const
	{
		sf::RectangleShape shape(collider.getVector());
		shape.setPosition(collider.position);
		shape.setOutlineThickness(2);
		shape.setOutlineColor(isYellow ? sf::Color::Yellow : sf::Color::Red);
		
		rt.draw(shape);
	}
	
	#endif // DRAW_PLATFORMS
};


class Room
{
	WallActor 	wall;
	bool 		collidableBounds;
	
public:
	
	void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
	{
		wall.draw(target, states);
	}
	
	Room(const Rect<double>& rect_, const WallType& wallType)
		: wall(wallType), collidableBounds(false)
	{
		wall.setPosition(rect_.position);
		wall.setSize(rect_.size);
		
	}
	Room(const Rect<double>& rect_, const WallType& wallType, std::vector<Platform>& platformsCollection)
		: wall(wallType), collidableBounds(false)
	{
		wall.setPosition(rect_.position);
		wall.setSize(rect_.size);
		
		Rect<double> tempRect = wall.getRect();
        platformsCollection.emplace_back(tempRect.getUpperLeft(),  tempRect.size.x, false);
        platformsCollection.emplace_back(tempRect.getBottomLeft(), tempRect.size.x, false);
        platformsCollection.emplace_back(tempRect.getUpperLeft(),  tempRect.size.y, true);
        platformsCollection.emplace_back(tempRect.getUpperRight(), tempRect.size.y, true);
		
	}
	
	
};

	
std::vector<Room> rooms;
std::vector<Platform> platforms;

#endif // ROOM_HPP_INCLUDED
