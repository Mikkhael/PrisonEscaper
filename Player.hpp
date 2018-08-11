#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "Object.hpp"
#include "LightEmitter.hpp"
#include "Keyboard.hpp"
#include "Room.hpp"
#include "Cannon.hpp"


class Player : public AnimatedSpriteActor
{
	double speed = 100;
	double shootForce = 500;
	double jumpForce;
	double reactionForce = 500;
	
	bool isInFreefall = false;
	
	static constexpr int colliderWidth = 4;
	static constexpr int colliderHeight = 15;
	static constexpr int defaultLightEmitterRdius = 100;
	
	PointLightEmitter* lightEmitter;
	
	inline Vector2d getCenter(){return getPosition() + Vector2d(PlayerSprite::width / 2, PlayerSprite::height / 2);}
	inline Vector2d getShootOrigin(){return getCenter();}
	inline Vector2d getLightEmitterOrigin(){return getCenter();}
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
		    if(!isInFreefall)
            {
                step += Vectors::foreward;
            }
			stateManager.isWalking = true;
			stateManager.isTurnedRight = true;
		}
		if(Controls::isPressed(Action::left))
		{
			if(!isInFreefall)
            {
                step += Vectors::backward;
            }
			stateManager.isWalking = true;
			stateManager.isTurnedRight = false;
		}
		
		if(!stateManager.isInAir && Controls::isPressed(Action::jump))
		{
			velocity += Vectors::down * jumpForce;
		}
		
		if(Controls::isTapped(Action::shoot))
		{
			Vector2d vel = (Controls::getMouseView() - getShootOrigin()).resize(shootForce);
			Vector2d pos = getShootOrigin();
			if(Cannonball::shoot(pos, vel))
            {
                isInFreefall = true;
                velocity += vel.resize((stateManager.isInAir ? -1 : -0.3) * reactionForce);
            }
		}
		step *= speed * deltaTime;
		
		updateSubstepKinematics(deltaTime, step, 2, [this](double deltaTime)
        {
            Vector2d shift = moveOutOfWalls(platforms);
            stateManager.isInAir = shift.y >= 0;
        });
        
		if(!stateManager.isInAir)
		{
		    velocity.x = 0;
		    isInFreefall = false;
		}
            
		stateManager.manageState(*this);
		
		
		lightEmitter->setPosition(getLightEmitterOrigin());
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
		setCollider(Rect<double>(
                           (PlayerSprite::width - colliderWidth) / 2,
                           PlayerSprite::height - colliderHeight,
                           colliderWidth,
                           colliderHeight
                           ));
		mass = 500;
		jumpForce = std::sqrt(mass * (colliderHeight) * 2);
		
		lightEmitter = new PointLightEmitter(defaultLightEmitterRdius);
		LightEmitter::spawn(lightEmitter);
    }
    
    virtual ~Player(){
        if(lightEmitter)
        {
            LightEmitter::despawn(lightEmitter);
        }
    }
		
};

#endif // PLAYER_HPP_INCLUDED
