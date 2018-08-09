#ifndef CANNON_HPP_INCLUDED
#define CANNON_HPP_INCLUDED

#include "Object.hpp"


class Cannonball : public SpriteActor, public ActorCollection<Cannonball>
{
public:
	
	
	
	void update(double deltaTime)
	{
	    updateSubstepKinematics(deltaTime, 2, [this](double deltaTime)
        {
            moveOutOfWalls(platforms);
        });
	}
		
	Cannonball(const Vector2d& position_ = Vectors::null, const Vector2d& velocity_ = Vectors::null)
		: SpriteActor(textureManager.get("assets/creatures/player.bmp"), {0, 32, 8, 8})
	{
		mass = 500;
		velocity = velocity_;
		setPosition(position_);
		//setCollider(Circle<double>(Vector2d(2, 2), 2));
		setCollider(Rect<double>({0,0}, {4,4}));
	}
	
	virtual ~Cannonball(){};
};

//std::vector<Cannonball*> ActorCollection<Cannonball>::list;

#endif // CANNON_HPP_INCLUDED
