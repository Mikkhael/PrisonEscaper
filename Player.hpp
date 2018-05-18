#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Object.hpp"
#include "Keyboard.hpp"
#include "Room.hpp"


class Player : public AnimatedSpriteActor
{
	
	double drag = 1;
	double speed = 100;
	
public:
	
	
	virtual void update(double deltaTime)
	{
	    
	    
		AnimatedSpriteActor::update(deltaTime);
		
		if(Controls::isPressed(Action::right))
		{
			velocity += Vectors::foreward	* speed * deltaTime;
		}
		if(Controls::isPressed(Action::left))
		{
			velocity += Vectors::backward 	* speed * deltaTime;
		}
		if(Controls::isPressed(Action::up))
		{
			velocity += Vectors::down 		* speed * deltaTime;
		}
		if(Controls::isPressed(Action::down))
		{
			velocity += Vectors::up		 	* speed * deltaTime;
		}
				
		updateKinematics(deltaTime, drag);
		
		handleAllCollisions<Player, std::vector<Platform> >(*this, platforms.begin(), platforms.end(), [](Collision::Result& result, Player& player, Platform& platform)
                      {
                          if(!platform.isYellow)
                          {
                              Rect<double> rect = reinterpret_cast<ShapeCollider<Rect<double> >* >(player.collider)->getPositionedCollider();
                              std::cout<<rect.getUpperLeft()<<" -- "<<rect.getUpperRight()<<std::endl<<rect.getBottomLeft()<<" -- "<<rect.getBottomRight()<<std::endl<<"with"<<std::endl<<platform.collider.position<<"  "<<platform.collider.length<<std::endl<<std::endl;
                              
                              
                          }
                            platform.isYellow = true;
                      });
		
	}
	
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
	{
		AnimatedSpriteActor::draw(target, states);
	}

	
	
	
	
	
	
	Player(Vector2d position)
		: AnimatedSpriteActor(AnimatedSpritePresets::PlayerIdle)
	{
		setPosition(position);
		isKinematic = true;
		setCollider(Rect<double>(0,0,16,16));
	    Rect<double> rect = reinterpret_cast<ShapeCollider<Rect<double> >* >(this->collider)->getPositionedCollider();
        std::cout<<rect.getUpperLeft()<<" -- "<<rect.getUpperRight()<<std::endl<<rect.getBottomLeft()<<" -- "<<rect.getBottomRight()<<std::endl<<std::endl;
	}
		
};

#endif // PLAYER_HPP_INCLUDED
