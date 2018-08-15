#ifndef CANNON_HPP_INCLUDED
#define CANNON_HPP_INCLUDED

#include "Object.hpp"
#include "WallTurret.hpp"

class Cannonball : public SpriteActor, public ActorCollection<Cannonball>
{
    
    static constexpr int colliderDiameter    = 5;
    static constexpr int colliderRadius      = colliderDiameter / 2;
    
public:
	
	
	
	void update(double deltaTime)
	{
	    updateSubstepKinematics(deltaTime, 2, [this](double deltaTime)
        {
            WallTurret::iterate([&](WallTurret& wt)
            {
                handleCollision(*this, wt, [&](Collision::Result result, Cannonball& cannonball, WallTurret& wallTurret)
                {
                    wallTurret.setPosition({-100, -100});
                });
                return false;
            });
            moveOutOfWalls(platforms);
            
        });
	}
	
	static bool shoot(const Vector2d& position_ = Vectors::null, const Vector2d& velocity_ = Vectors::null)
	{
	    if(Room::isPointInside(position_))
        {
            spawn(new Cannonball(position_, velocity_));
            return true;
        }
        return false;
	}
		
	Cannonball(const Vector2d& position_ = Vectors::null, const Vector2d& velocity_ = Vectors::null)
		: SpriteActor(textureManager.get(CannonballSprite::path, false), CannonballSprite::rect)
	{
		mass = 500;
		velocity = velocity_;
		setPosition(position_);
		sprite.setOrigin({colliderRadius + 1, colliderRadius + 1});
		setCollider(Circle<double>(Vector2d(0, 0), colliderRadius));
		//setCollider(Rect<double>(Vector2d(-colliderRadius, -colliderRadius), Vector2d(colliderRadius, colliderRadius)));
	}
	
	virtual ~Cannonball(){};
};

//std::vector<Cannonball*> ActorCollection<Cannonball>::list;

#endif // CANNON_HPP_INCLUDED
