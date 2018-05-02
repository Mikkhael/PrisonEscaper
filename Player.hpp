#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Object.hpp"
#include "Keyboard.hpp"
#include "Room.hpp"


class Player : public AnimatedSpriteActor
{
	
	double drag = 0.01;
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
		
		std::cout<< getPosition().x << "\t\t" << getPosition().y <<std::endl;
				
		updateKinematics(deltaTime, drag);		
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
	}
		
};

#endif // PLAYER_HPP_INCLUDED
