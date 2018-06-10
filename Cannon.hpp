#ifndef CANNON_HPP_INCLUDED
#define CANNON_HPP_INCLUDED

#include "Object.hpp"


class Cannonball : public SpriteActor
{
	
public:
	
	static std::vector<Cannonball> cannonballs;
	
	
	void update(double deltaTime)
	{
		std::cout << "A";
		updateKinematics(deltaTime);
		std::cout << "B";
		moveOutOfWalls(platforms);
		std::cout << "C\n";
	}
	
	static void spawn(const Vector2d& position_, const Vector2d& velocity_)
	{
		cannonballs.emplace_back(position_, velocity_);
	}
	
	static void updateAll(double deltaTime)
	{
		for(auto& cannonball : cannonballs)
		{
			cannonball.update(deltaTime);
		}
	}
	
	static void drawAll(sf::RenderTarget& rt, const sf::RenderStates& states = sf::RenderStates::Default)
	{
		for(auto& cannonball : cannonballs)
		{
			cannonball.draw(rt, states);
		}
	}
		
	Cannonball(const Vector2d& position_ = Vectors::null, const Vector2d& velocity_ = Vectors::null)
		: SpriteActor(textureManager.get("assets/creatures/player.bmp"), {0, 32, 8, 8})
	{
		mass = 500;
		velocity = velocity_;
		setPosition(position_);
		setCollider(Rect<double>(Vector2d(4, 4), Vector2d(4, 4)));
	}
	
	virtual ~Cannonball(){};
};

std::vector<Cannonball> Cannonball::cannonballs;

#endif // CANNON_HPP_INCLUDED
