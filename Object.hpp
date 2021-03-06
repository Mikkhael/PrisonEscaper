#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <typeinfo>
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
    virtual void updateCollider()
    {
        if(collider)
        {
            collider->updateCollider(getPosition(), getScale(), getRotation());
        }
    }
    
    Actor* parent = nullptr;
public:
	Collider* 	collider;
	
	virtual const Collider* getCollider() const
	{
        return collider;
	}
	
	double 		mass;
	Vector2d 	velocity;
	
	void updateKinematics(double deltaTime, const Vector2d& step = Vectors::null)
	{
		velocity += globalGravity * mass * deltaTime;
		move(velocity * deltaTime + step);
		velocity *= (1-globalDrag*deltaTime);
	}
	
	template<class THandler>
	void updateSubstepKinematics(double deltaTime,const Vector2d& step, double maxShift, THandler handler)
	{
	    updateSubstepKinematics(deltaTime, maxShift, handler, step);
	}
	
	template<class THandler>
	void updateSubstepKinematics(double deltaTime, double maxShift, THandler handler, const Vector2d& step = Vectors::null)
	{
		Vector2d fullShift, subShift, newStep = step, newVelocity;
		double   subDeltaTime, newDeltaTime = deltaTime;
		unsigned int sub;
        
		do
        {   
            newVelocity = velocity + globalGravity * mass * newDeltaTime;
            
            fullShift    = newVelocity * newDeltaTime + newStep;
            if(fullShift.magnatudeSquared() == 0)
                break;
            sub          = std::ceil(fullShift.magnatude() / maxShift);
            subDeltaTime = newDeltaTime / sub;
            subShift     = fullShift    / sub;
            
            velocity += globalGravity * mass * subDeltaTime;
            move(subShift);
            velocity *= (1-globalDrag*subDeltaTime);
            
            handler(subDeltaTime);
            
            newDeltaTime -= subDeltaTime;
            newStep      =  newStep - newStep / sub;
            
        }while(sub > 1);
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
    
    void removeCollider()
    {
        if(collider)
        {
            delete collider;
			collider = nullptr;
        }
    }
    
    Vector2d moveOutOfWalls(std::vector<Platform>& platforms)
    {
    	Vector2d shift(0,0);
    	handleAllCollisions(*this, platforms.begin(), platforms.end(), [&shift](Collision::Result& result, Actor& actor, Platform& platform)
        {
            if(platform.collider.isVertical)
			{
                
				if(std::abs(result.) <= 1)
                {
					//return;
                }
				actor.velocity.x = 0;
                //std::cout << result.getPenetrationVector() << std::endl;
			}
			else
			{
				if(std::abs(result.distance) <= 1)
                {
					//return;
                }
				actor.velocity.y = 0;
                //std::cout << result.getPenetrationVector() << std::endl;
			}
            shift += moveOutOfWall(result, actor);
        });
        return shift;
    }
    
	Actor()
		: 	collider(nullptr),
			mass(0),
			velocity(Vectors::null)
	{
	}	
	
	Actor(Actor&& a) noexcept
		: 	collider(a.collider),
			mass(a.mass),
			velocity(a.velocity)
			
	{
		a.collider = nullptr;
	}
	
	Actor(const Actor& a) 
		: 	collider(nullptr),
			mass(a.mass),
			velocity(a.velocity)
			
	{
		if(a.collider)
		{
			collider = a.collider->clonePtr();
		}
	}
	
	virtual ~Actor()
	{
		removeCollider();
	}
	
};

class SimpleActor : public Actor
{
    Transform transform;
public:
    
    virtual void setPosition (const Vector2d& 	position)
	{
		transform.position = position;
	}
	virtual void setScale	 (const Vector2d&	scale)
	{
		transform.scale = scale;
	}
	virtual void setRotation (double 			rotation)
	{
		transform.rotation = rotation;
	}
	
	virtual Vector2d getPosition() 	const
	{
		return transform.position;
	}
	virtual Vector2d getScale() 	const
	{
		return transform.scale;
	}
	virtual double	 getRotation()	const
	{
		return transform.rotation;
	}
	
	SimpleActor()
        : transform()
	{}
	
	virtual ~SimpleActor(){}
};

template<class T>
class Collection
{
protected:
    
    static std::vector<T*> list;
public:
    
    static bool spawn(T* element)
    {
        //std::cout << "Spawned " << typeid(T).name() << std::endl;
        for(auto it = list.begin(); it < list.end(); it++)
        {
            if(*it == element)
            {
                return false;
            }
        }
        list.push_back(element);
        return true;
    }
    
    static bool despawn(T* element)
    {
        for(auto it = list.begin(); it < list.end(); it++)
        {
            if(*it == element)
            {
                delete *it;
                list.erase(it);
                return true;
            }
        }
        return false;
    }
    
    static void despawnAll()
    {
        for(auto it = list.begin(); it != list.end(); it++)
        {
            delete *it;
        }
        list.erase(list.begin(), list.end());
    }
    
    template<class Thandler>
    static void iterate(Thandler handler)
    {
        bool end = false;
        for(auto it = list.begin(); it != list.end() && !end; it++)
        {
            end = handler(*(*it));
        }
    }
};

template<class T>
std::vector<T*> Collection<T>::list;

template<class T>
class ActorCollection : public Collection<T>
{
public:
    static void drawAll(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default)
    {
        for(auto actor : Collection<T>::list)
        {
            actor->draw(target, states);
        }
    }
    static void updateAll(double deltaTime)
    {
        for(auto actor : Collection<T>::list)
        {
            actor->update(deltaTime);
        }
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
