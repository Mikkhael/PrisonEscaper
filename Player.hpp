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
	
public:
	
	struct StateManager
	{
	private:
	public:
		bool isTurnedRight  = true;
		bool isWalking 	    = false;
		bool isInAir		= true;
		
		void manageState(Player& player)
		{
			if(isWalking)
			{
				player.sprite.setPreset(AnimatedSpritePresets::PlayerWalk);
			}
			else if(isInAir)
			{
				player.sprite.setPreset(AnimatedSpritePresets::PlayerFall);
			}
			else
			{
				player.sprite.setPreset(AnimatedSpritePresets::PlayerIdle);
			}
			
			player.sprite.flipX = !isTurnedRight;
		}
				
	}	stateManager;
	
	virtual void update(double deltaTime)
	{
	    Vector2d step;
	    
		AnimatedSpriteActor::update(deltaTime);
		
		stateManager.isWalking = false;
		if(Controls::isPressed(Action::right))
		{
			step += Vectors::foreward;
			stateManager.isWalking = true;
		}
		if(Controls::isPressed(Action::left))
		{
			step += Vectors::backward;
			stateManager.isWalking = true;
		}
		
		stateManager.isTurnedRight = step.x > 0;
		step *= speed * deltaTime;
		
		if(!stateManager.isInAir && Controls::isTapped(Action::jump))
		{
			velocity += Vectors::down * jumpForce;
		}
				
		updateKinematics(deltaTime, drag, gravity, step);
		Vector2d shift = moveOutOfWalls(platforms);
		
		stateManager.isInAir = shift.y > 0;
		
		stateManager.manageState(*this);
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
