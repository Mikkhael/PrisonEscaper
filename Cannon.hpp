#ifndef CANNON_HPP_INCLUDED
#define CANNON_HPP_INCLUDED

#include "Object.hpp"


class Cannonball : public SpriteActor, public ActorCollection<Cannonball>
{
public:
	
	
	
	void update(double deltaTime)
	{
		updateKinematics(deltaTime);
		moveOutOfWalls(platforms);
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

//std::vector<Cannonball*> ActorCollection<Cannonball>::list;

#endif // CANNON_HPP_INCLUDED
