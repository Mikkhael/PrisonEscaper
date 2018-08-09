#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Object.hpp"
#include "Keyboard.hpp"
#include "Room.hpp"
#include "Cannon.hpp"


class Player : public AnimatedSpriteActor
{
	double speed = 100;
	double shootForce = 500;
	double jumpForce;
	
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
			stateManager.isTurnedRight = true;
		}
		if(Controls::isPressed(Action::left))
		{
			step += Vectors::backward;
			stateManager.isWalking = true;
			stateManager.isTurnedRight = false;
		}
		
		step *= speed * deltaTime;
		
		if(!stateManager.isInAir && Controls::isPressed(Action::jump))
		{
			velocity += Vectors::down * jumpForce;
		}
		
		if(Controls::isTapped(Action::shoot))
		{
			Vector2d vel = (Controls::getMouseView() - getPosition()).resize(shootForce);
			Cannonball::spawn(new Cannonball(getPosition(), vel));
		}
		
		
		updateSubstepKinematics(deltaTime, step, 2, [this](double deltaTime)
        {
            Vector2d shift = moveOutOfWalls(platforms);
            stateManager.isInAir = shift.y >= 0;
        });
        
		stateManager.manageState(*this);
	}
	
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
	{
		AnimatedSpriteActor::draw(target, states);
	}
	
	Player(const Vector2d& position)
		: AnimatedSpriteActor(AnimatedSpritePresets::PlayerIdle)
	{
		setPosition(position);
		isKinematic = true;
		setCollider(Rect<double>(6,1,4,15));
		mass = 500;
		jumpForce = std::sqrt(mass * (16 * 5) * 2);
    }
    
    virtual ~Player(){}
		
};

#endif // PLAYER_HPP_INCLUDED
