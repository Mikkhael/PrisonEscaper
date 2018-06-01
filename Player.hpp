#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Object.hpp"
#include "Keyboard.hpp"
#include "Room.hpp"


class Player : public AnimatedSpriteActor
{
	
	double drag = 1;
	double speed = 100;
	Vector2d gravity = Vector2d(0, 500);
	double jumpForce = std::sqrt(500 * (16 * 3) * 2);
	
	bool inAir = true;
	
public:	
	virtual void update(double deltaTime)
	{
	    Vector2d step;
	    
		AnimatedSpriteActor::update(deltaTime);
		
		if(Controls::isPressed(Action::right))
		{
			step += Vectors::foreward	* speed * deltaTime;
		}
		if(Controls::isPressed(Action::left))
		{
			step += Vectors::backward 	* speed * deltaTime;
		}
		if(Controls::isPressed(Action::up))
		{
			step += Vectors::down 		* speed * deltaTime;
		}
		if(Controls::isPressed(Action::down))
		{
			step += Vectors::up		 	* speed * deltaTime;
		}
		
		if(inAir && Controls::isTapped(Action::jump))
		{
			velocity += Vectors::down * jumpForce;
		}
				
		updateKinematics(deltaTime, drag, gravity, step);
		Vector2d shift = moveOutOfWalls(platforms);
		
		inAir = shift.y < 0;
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
    }
		
};

#endif // PLAYER_HPP_INCLUDED
