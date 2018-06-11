#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include "Vectors.hpp"
#include "TextureManager.hpp"
#include "Animations.hpp"
#include "Colisions.hpp"
#include "Shapes.hpp"
#include "PLatform.hpp"

Vector2d globalGravity 	= Vectors::up;
double 	 globalDrag		= 1;

class Actor : public Transformable, public Collidable
{
protected:
    void updateCollider()
    {
        if(collider)
        {
            collider->updateCollider(getPosition(), getScale(), getRotation());
        }
    }
public:
	Collider* 	collider;
	
	virtual const Collider* getCollider() const
	{
        return collider;
	}
	
	bool		isKinematic = false;
	double 		mass		= 0;
	Vector2d 	velocity	= Vectors::null;
	
	void updateKinematics(double deltaTime, const Vector2d& step = Vectors::null, double drag = globalDrag, const Vector2d& gravity = globalGravity)
	{
		velocity += gravity * mass * deltaTime;
		move(velocity * deltaTime + step);
		velocity *= (1-drag*deltaTime);
	}
	
	virtual void update(double deltaTime)
	{
		return;
	}
	
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
	{
		return;
	}
	
	
	Collision::Result testCollision(const Collider& collider_) const
	{
		if(collider)
		{
			return collider->test(collider_);
		}
		return Collision::Result(false);
	}
	Collision::Result testCollision(const Collider* collider_) const
	{
		if(collider && collider_)
		{
			return collider->test(*collider_);
		}
		return Collision::Result(false);
	}
	Collision::Result testCollision(const Actor& actor_) const
	{
		if(collider && actor_.collider)
		{
			return collider->test(*actor_.collider);
		}
		return Collision::Result(false);
	}
	
	template<class T>
    void setCollider(const T& shape)
    {
        if(collider)
        {
            delete collider;
        }
        collider = new ShapeCollider<T>(shape);
    }
    
    Vector2d moveOutOfWalls(std::vector<Platform>& platforms)
    {
    	Vector2d shift;
    	handleAllCollisions(*this, platforms.begin(), platforms.end(), [&shift](Collision::Result& result, Actor& actor, Platform& platform)
        {
            if(platform.collider.isVertical)
			{
				if(std::abs(result.yPenetration) <= 0.5)
					return;
				actor.velocity.x = 0;
			}
			else
			{
				if(std::abs(result.xPenetration) <= 0.5)
					return;
				actor.velocity.y = 0;
			}
            shift += moveOutOfWall(result, actor, platform.collider);
        });
        return shift;
    }
    
    
    
    
	
	Actor()
		: collider(nullptr)
	{}	
	
	
	virtual ~Actor()
	{
		if(collider)
		{
			delete collider;
		}
		std::cout << "Dest" << std::endl;
	}
	
};


class SpriteActor : public Actor
{	
public:
		
	sf::Sprite sprite;
	
	// Transformable
	virtual void setPosition (const Vector2d& 	position)
	{
		sprite.setPosition(position);
		updateCollider();
	}
	virtual void setScale	 (const Vector2d&	scale)
	{
		sprite.setScale(scale);
		updateCollider();
	}
	virtual void setRotation (double 			rotation)
	{
		sprite.setRotation(rotation);
		updateCollider();
	}
	
	virtual Vector2d getPosition() 	const
	{
		return sprite.getPosition();
	}
	virtual Vector2d getScale() 	const
	{
		return sprite.getScale();
	}
	virtual double	 getRotation()	const
	{
		return sprite.getRotation();
	}
	////////
	
	void setSize(const Vector2i& newSize)
	{
		sprite.setTextureRect(sf::IntRect(sprite.getTextureRect().left, sprite.getTextureRect().top, newSize.x, newSize.y));
	}
	Vector2i getSize() const
	{
		return Rect<int>(sprite.getTextureRect()).size;
	}
	
	void setOffset(const Vector2i& newOffset)
	{
		sprite.setTextureRect(sf::IntRect(newOffset.x, newOffset.y, sprite.getTextureRect().width, sprite.getTextureRect().height));
	}
	Vector2i getOffset() const
	{
		return Rect<int>(sprite.getTextureRect()).position;
	}
	
	
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
	{
		target.draw(sprite, states);
	}
	
	
	
	SpriteActor(const sf::Texture& texture, const sf::IntRect& rect)
		: sprite(texture)
	{
		sprite.setTextureRect(rect);
	}
	
	virtual ~SpriteActor(){}
	
};

class AnimatedSpriteActor : public Actor
{
public:
	
	AnimatedSprite sprite;
	
	// Transformable
	virtual void setPosition (const Vector2d& 	position)
	{
		sprite.setPosition(position);
		updateCollider();
	}
	virtual void setScale	 (const Vector2d&	scale)
	{
		sprite.setScale(scale);
		updateCollider();
	}
	virtual void setRotation (double 			rotation)
	{
		sprite.setRotation(rotation);
		updateCollider();
	}
	
	virtual Vector2d getPosition() 	const
	{
		return sprite.getPosition();
	}
	virtual Vector2d getScale() 	const
	{
		return sprite.getScale();
	}
	virtual double	 getRotation()	const
	{
		return sprite.getRotation();
	}
	//////////
	
	Vector2i getSize() const
	{
		return Rect<int>(sprite.getTextureRect()).size;
	}
	
	Vector2i getOffset() const
	{
		return Rect<int>(sprite.getTextureRect()).position;
	}
	
	
	
	virtual void update(double deltaTime)
	{
		sprite.updateFrame(deltaTime);
	}
	
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default) const
	{
		target.draw(sprite, states);
	}
	
	
	AnimatedSpriteActor(const AnimatedSpritePreset& preset)
		: sprite(preset)
	{
		
	}
	
	virtual ~AnimatedSpriteActor(){}
};


#endif // OBJECT_HPP_INCLUDED
